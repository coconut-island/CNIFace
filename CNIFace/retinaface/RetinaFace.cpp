//
// Created by Abel Lee on 2022/5/28.
//

#include "RetinaFace.h"

#include <iostream>
#include <fstream>

#include <tvm/runtime/registry.h>

#include "../utils/ImageUtil.h"


void clip_boxes(Anchor &anchor, float width, float height) {
    if (anchor.x < 0) {
        anchor.x = 0;
    }
    if (anchor.y < 0) {
        anchor.y = 0;
    }
    if (anchor.x + anchor.w > width - 1) {
        anchor.w = width - anchor.x - 1;
    }
    if (anchor.y + anchor.h > height - 1) {
        anchor.y = height - anchor.h -1;
    }
}

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

RetinaFace::RetinaFace(const string &model_dir_path) {
    std::vector<int> cpu_devices = {0};
    init(model_dir_path, m_default_model_name, cpu_devices);
}

RetinaFace::RetinaFace(const string &model_dir_path, const std::vector<int> &cpu_devices) {
    init(model_dir_path, m_default_model_name, cpu_devices);
}

RetinaFace::RetinaFace(const std::string &model_dir_path, const string &model_name, const std::vector<int> &cpu_devices) {
    init(model_dir_path, model_name, cpu_devices);
}

RetinaFace::~RetinaFace() = default;

void RetinaFace::init(const string &model_dir_path, const string &model_name, const std::vector<int> &cpu_devices) {
    if (!m_handles.empty()) {
        return;
    }

    string so_lib_path = model_dir_path + model_name  + ".so";
    tvm::runtime::Module mod_syslib = tvm::runtime::Module::LoadFromFile(so_lib_path);

    // json graph
    std::ifstream json_in(model_dir_path + model_name + ".json", std::ios::in);
    std::string json_data((std::istreambuf_iterator<char>(json_in)), std::istreambuf_iterator<char>());
    json_in.close();

    // parameters in binary
    std::ifstream params_in(model_dir_path + model_name + ".params", std::ios::binary);
    std::string params_data((std::istreambuf_iterator<char>(params_in)), std::istreambuf_iterator<char>());
    params_in.close();

    for (auto& cpu_device : cpu_devices) {
        tvm::runtime::Module mod = (*tvm::runtime::Registry::Get("tvm.graph_executor.create"))(json_data, mod_syslib, m_device_type, cpu_device);
        this->m_handles.emplace_back(std::make_shared<tvm::runtime::Module>(mod));

        TVMByteArray params_arr;
        params_arr.data = params_data.c_str();
        params_arr.size = params_data.length();
        tvm::runtime::PackedFunc load_params = mod.GetFunction("load_params");
        load_params(params_arr);
    }
}

vector<Anchor> RetinaFace::detect(uint8_t* bgr_img, int img_width, int img_height, float score_threshold = 0.5) {
    float im_ratio = (float)img_height / (float)img_width;
    float model_ratio = (float)m_input_height / (float)m_input_width;

    int new_height;
    int new_width;
    if (im_ratio > model_ratio) {
        new_height = m_input_height;
        new_width = (int)((float)new_height / im_ratio);
    } else {
        new_width = m_input_width;
        new_height = (int)((float)new_width * im_ratio);
    }

    auto* resized_img = (uint8_t*)malloc(new_width * new_height * 3 * sizeof(uint8_t));
    ImageUtil::bilinear_resize(bgr_img, resized_img, img_width, img_height, new_width, new_height);

    auto* resized_padding_img = (uint8_t*)malloc(m_input_elements * sizeof(uint8_t));
    // right padding
    if (new_width < m_input_width) {
        for (int j = 0; j < m_input_height; ++j) {
            for (int i = 0; i < m_input_width; ++i) {
                if (i < new_width) {
                    resized_padding_img[j * m_input_width * 3 + 3 * i] = resized_img[j * new_width * 3 + 3 * i];
                    resized_padding_img[j * m_input_width * 3 + 3 * i + 1] = resized_img[j * new_width * 3 + 3 * i + 1];
                    resized_padding_img[j * m_input_width * 3 + 3 * i + 2] = resized_img[j * new_width * 3 + 3 * i + 2];
                }
            }
        }
    } else {
        // bottom padding
        memcpy(resized_padding_img, resized_img, new_width * new_height * 3 * sizeof(uint8_t));
    }

    auto* input_data = (float*)malloc(m_input_size);
    for (int i = 0; i < m_input_width * m_input_height; i++) {
        input_data[i] = m_scale * ((float)resized_padding_img[i * 3 + 2] - m_mean);
        input_data[i + m_input_width * m_input_height] = m_scale * ((float)resized_padding_img[i * 3 + 1] - m_mean);
        input_data[i + m_input_width * m_input_height * 2] = m_scale * ((float)resized_padding_img[i * 3] - m_mean);
    }

    int cur_cpu_device_idx = m_cur_cpu_device_idx;
    auto *mod = (tvm::runtime::Module *)m_handles[m_cur_cpu_device_idx].get();
    if (++m_cur_cpu_device_idx >= m_handles.size()) m_cur_cpu_device_idx = 0;

    tvm::runtime::PackedFunc set_input = mod->GetFunction("set_input");
    tvm::runtime::PackedFunc load_params = mod->GetFunction("load_params");
    tvm::runtime::PackedFunc run = mod->GetFunction("run");
    tvm::runtime::PackedFunc get_output = mod->GetFunction("get_output");

    DLTensor* tvm_input_data;
    int64_t in_shape[4] = { 1, m_input_channel, m_input_height, m_input_width };
    TVMArrayAlloc(in_shape, m_in_ndim, m_dtype_code, m_dtype_bits, m_dtype_lanes, m_device_type, cur_cpu_device_idx, &tvm_input_data);
    TVMArrayCopyFromBytes(tvm_input_data, input_data, m_input_size);

    set_input(m_input_name, tvm_input_data);

    run();

    vector<Anchor> anchors;

    for (int i = 0; i < m_feat_stride_fpn.size(); ++i) {
        int stride = m_feat_stride_fpn[i];
        tvm::runtime::NDArray tvm_scores_output = get_output(i);
        tvm::runtime::NDArray tvm_bbox_preds_output = get_output(i + m_output_sym);
        tvm::runtime::NDArray tvm_kps_preds_output = get_output(i + m_output_sym * 2);

        auto scores_output_shape = m_out_shapes[i];
        auto bbox_preds_output_shape = m_out_shapes[i + m_output_sym];
        auto kps_preds_output_shape = m_out_shapes[i + m_output_sym * 2];

        auto scores_output_size = scores_output_shape[0] * scores_output_shape[1] * sizeof(float);
        auto bbox_preds_output_size = bbox_preds_output_shape[0] * bbox_preds_output_shape[1] * sizeof(float);
        auto kps_preds_output_size = kps_preds_output_shape[0] * kps_preds_output_shape[1] * sizeof(float);

        auto *scores = (float *) malloc(scores_output_size);
        auto *bbox_preds = (float *) malloc(bbox_preds_output_size);
        auto *kps_preds = (float *) malloc(kps_preds_output_size);

        tvm_scores_output.CopyToBytes(scores, scores_output_size);
        tvm_bbox_preds_output.CopyToBytes(bbox_preds, bbox_preds_output_size);
        tvm_kps_preds_output.CopyToBytes(kps_preds, kps_preds_output_size);

        auto strideF = (float)stride;
        for (int j = 0; j < bbox_preds_output_shape[0] * bbox_preds_output_shape[1]; ++j) {
            bbox_preds[j] = bbox_preds[j] * strideF;
        }

        for (int j = 0; j < kps_preds_output_shape[0] * kps_preds_output_shape[1]; ++j) {
            kps_preds[j] = kps_preds[j] * strideF;
        }

        int height = floor(m_input_height / stride);
        int width = floor(m_input_width / stride);

        for (int idx = 0; idx < scores_output_shape[0] * scores_output_shape[1]; ++idx) {
            if (scores[idx] < score_threshold) {
                continue;
            }
            int x = (idx % (width * m_num_anchors)) / m_num_anchors;
            int y = idx / (width * m_num_anchors);
            float anchor_center[2] = { (float)x * strideF, (float)y * strideF};

            int bbox_preds_dim1 = m_out_shapes[i + m_output_sym][1]; // 4
            int kps_preds_dim1 = m_out_shapes[i + m_output_sym * 2][1]; // 10

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

        free(scores);
        free(bbox_preds);
        free(kps_preds);
    }

    vector<Anchor> nms_anchors;
    nms(anchors, m_nms_thresh, nms_anchors);

    float det_scale = float(new_height) / (float)img_height;
    for (auto& anchor: nms_anchors) {
        anchor.x = anchor.x / det_scale;
        anchor.y = anchor.y / det_scale;
        anchor.w = anchor.w / det_scale - anchor.x;
        anchor.h = anchor.h / det_scale - anchor.y;

        clip_boxes(anchor, img_width, img_height);

        for (auto& kp : anchor.kps) {
            kp = kp / det_scale;
        }
    }

    TVMArrayFree(tvm_input_data);
    free(resized_img);
    free(resized_padding_img);
    free(input_data);
    return nms_anchors;
}