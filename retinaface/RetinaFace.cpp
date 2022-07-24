//
// Created by Abel Lee on 2022/5/28.
//

#include "RetinaFace.h"

#include <iostream>
#include <fstream>

#include <tvm/runtime/registry.h>

#include "../utils/ImageUtil.h"


void RetinaFace::nms(std::vector<Anchor>& anchors, float threshold, std::vector<Anchor>& out_anchors) {
    if(anchors.empty()) {
        return;
    }

    out_anchors.clear();
    std::vector<size_t> idx(anchors.size());
    for(size_t i = 0; i < idx.size(); i++) {
        idx[i] = i;
    }

    sort(anchors.begin(), anchors.end(),[](const Anchor& a, const Anchor& b){return a.score > b.score;});

    while(!idx.empty()) {
        int _idx = idx[0];
        out_anchors.push_back(anchors[_idx]);

        std::vector<size_t> tmp = idx;
        idx.clear();
        for(int i = 1; i < tmp.size(); i++) {
            int tmp_idx = tmp[i];
            float inter_x1 = std::max(anchors[_idx].x, anchors[tmp_idx].x);
            float inter_y1 = std::max(anchors[_idx].y, anchors[tmp_idx].y);
            float inter_x2 = std::min(anchors[_idx].w, anchors[tmp_idx].w);
            float inter_y2 = std::min(anchors[_idx].h, anchors[tmp_idx].h);

            float w = std::max((inter_x2 - inter_x1 + 1), 0.0F);
            float h = std::max((inter_y2 - inter_y1 + 1), 0.0F);

            float inter_area = w * h;
            float area_1 = (anchors[_idx].w - anchors[_idx].x + 1) * (anchors[_idx].h - anchors[_idx].y + 1);
            float area_2 = (anchors[tmp_idx].w - anchors[tmp_idx].x + 1) * (anchors[tmp_idx].h - anchors[tmp_idx].y + 1);
            float o = inter_area / (area_1 + area_2 - inter_area);
            if (o <= threshold) {
                idx.push_back(tmp_idx);
            }
        }
    }
}

RetinaFace::RetinaFace(const std::string &model_dir_path) {
    string so_lib_path = model_dir_path + model_name  + ".so";
    Module mod_syslib = Module::LoadFromFile(so_lib_path);

    // json graph
    std::ifstream json_in(model_dir_path + model_name + ".json", std::ios::in);
    std::string json_data((std::istreambuf_iterator<char>(json_in)), std::istreambuf_iterator<char>());
    json_in.close();

    // parameters in binary
    std::ifstream params_in(model_dir_path + model_name + ".params", std::ios::binary);
    std::string params_data((std::istreambuf_iterator<char>(params_in)), std::istreambuf_iterator<char>());
    params_in.close();

    tvm::runtime::Module mod = (*Registry::Get("tvm.graph_executor.create"))(json_data, mod_syslib, device_type, device_id);

    this->handle = std::make_shared<tvm::runtime::Module>(mod);

    TVMByteArray params_arr;
    params_arr.data = params_data.c_str();
    params_arr.size = params_data.length();
    tvm::runtime::PackedFunc load_params = mod.GetFunction("load_params");
    load_params(params_arr);
}

RetinaFace::~RetinaFace() = default;

vector<Anchor> RetinaFace::detect(uint8_t* rgb_img, int img_width, int img_height, float score_threshold = 0.5) {
    float im_ratio = (float)img_height / (float)img_width;
    float model_ratio = (float)input_height / (float)input_width;

    int new_height;
    int new_width;
    if (im_ratio > model_ratio) {
        new_height = input_height;
        new_width = (int)((float)new_height / im_ratio);
    } else {
        new_width = input_width;
        new_height = (int)((float)new_width * im_ratio);
    }

    auto* resized_img = (uint8_t*)malloc(new_width * new_height * 3 * sizeof(uint8_t));
    ImageUtil::bilinear_resize(rgb_img, resized_img, img_width, img_height, new_width, new_height);

    auto* resized_padding_img = (uint8_t*)malloc(input_elements * sizeof(uint8_t));
    memcpy(resized_padding_img, resized_img, new_width * new_height * 3 * sizeof(uint8_t));

    auto* input_data = (float*)malloc(input_size);
    for (int i = 0; i < input_width * input_height; i++) {
        input_data[i] = scale * ((float)resized_padding_img[i * 3] - mean);
        input_data[i + input_width * input_height] = scale * ((float)resized_padding_img[i * 3 + 1] - mean);
        input_data[i + input_width * input_height * 2] = scale * ((float)resized_padding_img[i * 3 + 2] - mean);
    }

    auto *mod = (Module *)handle.get();

    PackedFunc set_input = mod->GetFunction("set_input");
    PackedFunc load_params = mod->GetFunction("load_params");
    PackedFunc run = mod->GetFunction("run");
    PackedFunc get_output = mod->GetFunction("get_output");

    DLTensor* tvm_input_data;
    int64_t in_shape[4] = { 1, input_channel, input_height, input_width };
    TVMArrayAlloc(in_shape, in_ndim, dtype_code, dtype_bits, dtype_lanes, device_type, device_id, &tvm_input_data);
    TVMArrayCopyFromBytes(tvm_input_data, input_data, input_size);

    set_input(input_name, tvm_input_data);

    run();

    vector<Anchor> anchors;

    for (int i = 0; i < feat_stride_fpn.size(); ++i) {
        int stride = feat_stride_fpn[i];
        tvm::runtime::NDArray tvm_scores_output = get_output(i);
        tvm::runtime::NDArray tvm_bbox_preds_output = get_output(i + fmc);
        tvm::runtime::NDArray tvm_kps_preds_output = get_output(i + fmc * 2);

        auto scores_output_shape = out_shapes[i];
        auto bbox_preds_output_shape = out_shapes[i + fmc];
        auto kps_preds_output_shape = out_shapes[i + fmc * 2];

        auto scores_output_size = scores_output_shape[0] * scores_output_shape[1] * sizeof(float);
        auto bbox_preds_output_size = bbox_preds_output_shape[0] * bbox_preds_output_shape[1] * sizeof(float);
        auto kps_preds_output_size = kps_preds_output_shape[0] * kps_preds_output_shape[1] * sizeof(float);

        auto *scores = (float *) malloc(scores_output_size);
        auto *bbox_preds = (float *) malloc(bbox_preds_output_size);
        auto *kps_preds = (float *) malloc(kps_preds_output_size);

        memcpy(scores, tvm_scores_output->data, scores_output_size);
        memcpy(bbox_preds, tvm_bbox_preds_output->data, bbox_preds_output_size);
        memcpy(kps_preds, tvm_kps_preds_output->data, kps_preds_output_size);

        auto strideF = (float)stride;
        for (int j = 0; j < bbox_preds_output_shape[0] * bbox_preds_output_shape[1]; ++j) {
            bbox_preds[j] = bbox_preds[j] * strideF;
        }

        for (int j = 0; j < kps_preds_output_shape[0] * kps_preds_output_shape[1]; ++j) {
            kps_preds[j] = kps_preds[j] * strideF;
        }

        int height = floor(input_height / stride);
        int width = floor(input_width / stride);

        for (int idx = 0; idx < scores_output_shape[0] * scores_output_shape[1]; ++idx) {
            if (scores[idx] < score_threshold) {
                continue;
            }
            int x = (idx % (width * num_anchors)) / num_anchors;
            int y = idx / (width * num_anchors);
            float anchor_center[2] = { (float)x * strideF, (float)y * strideF};

            int bbox_preds_dim1 = out_shapes[i + fmc][1]; // 4
            int kps_preds_dim1 = out_shapes[i + fmc * 2][1]; // 10

            Anchor anchor;
            anchor.score = scores[idx];
            anchor.x = anchor_center[0] - bbox_preds[idx * bbox_preds_dim1];
            anchor.y = anchor_center[1] - bbox_preds[idx * bbox_preds_dim1 + 1];
            anchor.w = anchor_center[0] + bbox_preds[idx * bbox_preds_dim1 + 2];
            anchor.h = anchor_center[1] + bbox_preds[idx * bbox_preds_dim1 + 3];
            for (int j = 0; j < kps_preds_dim1; j+=2) {
                anchor.kps[j] = anchor_center[0] + kps_preds[idx * kps_preds_dim1 + j];
                anchor.kps[j + 1] = anchor_center[1] + kps_preds[idx * kps_preds_dim1 + j + 1];
            }

            anchors.emplace_back(anchor);
        }

//        TVMObjectFree(&tvm_scores_output);
//        TVMObjectFree(&tvm_bbox_preds_output);
//        TVMObjectFree(&tvm_kps_preds_output);
        free(scores);
        free(bbox_preds);
        free(kps_preds);
    }

    float det_scale = float(new_height) / (float)img_height;
    for (auto& anchor: anchors) {
        anchor.x = anchor.x / det_scale;
        anchor.y = anchor.y / det_scale;
        anchor.w = anchor.w / det_scale;
        anchor.h = anchor.h / det_scale;

        for (auto& kp : anchor.kps) {
            kp = kp / det_scale;
        }
    }

    vector<Anchor> nms_anchors;
    nms(anchors, nms_thresh, nms_anchors);

    TVMArrayFree(tvm_input_data);
    free(resized_img);
    free(resized_padding_img);
    free(input_data);
    return nms_anchors;
}