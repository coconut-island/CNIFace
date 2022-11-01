//
// Created by Abel Lee on 2022/7/24.
//

#include "MNetCov2.h"

#include <iostream>
#include <fstream>

#include <tvm/runtime/registry.h>

#include "../utils/ImageUtil.h"


void landmark_pred(anchor_box anchor, vector<float>& kps) {
    float width = anchor.x2 - anchor.x1 + 1;
    float height = anchor.y2 - anchor.y1 + 1;
    float ctr_x = anchor.x1 + 0.5f * (width - 1.0f);
    float ctr_y = anchor.y1 + 0.5f * (height - 1.0f);

    for(size_t j = 0; j < 5; j ++) {
        kps[j * 2] = kps[j * 2] * width + ctr_x;
        kps[j * 2 + 1] = kps[j * 2 + 1] * height + ctr_y;
    }
}


anchor_box bbox_pred(anchor_box anchor, const Anchor& regress) {
    anchor_box rect{};

    float width = anchor.x2 - anchor.x1 + 1;
    float height = anchor.y2 - anchor.y1 + 1;
    float ctr_x = anchor.x1 + 0.5 * (width - 1.0);
    float ctr_y = anchor.y1 + 0.5 * (height - 1.0);

    float pred_ctr_x = regress.x * width + ctr_x;
    float pred_ctr_y = regress.y * height + ctr_y;
    float pred_w = exp(regress.w) * width;
    float pred_h = exp(regress.h) * height;

    rect.x1 = pred_ctr_x - 0.5 * (pred_w - 1.0);
    rect.y1 = pred_ctr_y - 0.5 * (pred_h - 1.0);
    rect.x2 = pred_ctr_x + 0.5 * (pred_w - 1.0);
    rect.y2 = pred_ctr_y + 0.5 * (pred_h - 1.0);

    return rect;
}

Anchor whctrs(anchor_box anchor) {
    //Return width, height, x center, and y center for an anchor (window).
    Anchor win;
    win.w = anchor.x2 - anchor.x1 + 1;
    win.h = anchor.y2 - anchor.y1 + 1;
    win.x = anchor.x1 + 0.5f * (win.w - 1);
    win.y = anchor.y1 + 0.5f * (win.h - 1);

    return win;
}

anchor_box mkanchors(const Anchor& win) {
    //Given a vector of widths (ws) and heights (hs) around a center
    //(x_ctr, y_ctr), output a set of anchors (windows).
    anchor_box anchor{};
    anchor.x1 = win.x - 0.5 * (win.w - 1);
    anchor.y1 = win.y - 0.5 * (win.h - 1);
    anchor.x2 = win.x + 0.5 * (win.w - 1);
    anchor.y2 = win.y + 0.5 * (win.h - 1);

    return anchor;
}

vector<anchor_box> ratio_enum(anchor_box anchor, const vector<float>& ratios) {
    //Enumerate a set of anchors for each aspect ratio wrt an anchor.
    vector<anchor_box> anchors;
    for (float ratio : ratios) {
        Anchor win = whctrs(anchor);
        float size = win.w * win.h;
        float scale = size / ratio;

        win.w = std::round(sqrt(scale));
        win.h = std::round(win.w * ratio);

        anchor_box tmp = mkanchors(win);
        anchors.push_back(tmp);
    }

    return anchors;
}

vector<anchor_box> scale_enum(anchor_box anchor, const vector<int>& scales)
{
    //Enumerate a set of anchors for each scale wrt an anchor.
    vector<anchor_box> anchors;
    for (auto scale : scales) {
        Anchor win = whctrs(anchor);

        win.w = win.w * scale;
        win.h = win.h * scale;

        anchor_box tmp = mkanchors(win);
        anchors.push_back(tmp);
    }

    return anchors;
}

vector<anchor_box> generate_anchors(int base_size = 16, const vector<float>& ratios = {0.5, 1, 2},
                                    const vector<int>& scales = {8, 64}, int stride = 16, bool dense_anchor = false) {
    //Generate anchor (reference) windows by enumerating aspect ratios X
    //scales wrt a reference (0, 0, 15, 15) window.

    anchor_box base_anchor{};
    base_anchor.x1 = 0;
    base_anchor.y1 = 0;
    base_anchor.x2 = base_size - 1.0f;
    base_anchor.y2 = base_size - 1.0f;

    vector<anchor_box> ratio_anchors;
    ratio_anchors = ratio_enum(base_anchor, ratios);

    vector<anchor_box> anchors;
    for (auto & ratio_anchor : ratio_anchors) {
        vector<anchor_box> tmp = scale_enum(ratio_anchor, scales);
        anchors.insert(anchors.end(), tmp.begin(), tmp.end());
    }

    if(dense_anchor) {
        assert(stride % 2 == 0);
        vector<anchor_box> anchors2 = anchors;
        for(auto & i : anchors2) {
            i.x1 += stride / 2;
            i.y1 += stride / 2;
            i.x2 += stride / 2;
            i.y2 += stride / 2;
        }
        anchors.insert(anchors.end(), anchors2.begin(), anchors2.end());
    }

    return anchors;
}

vector<vector<anchor_box>> generate_anchors_fpn(bool dense_anchor = false, const vector<anchor_cfg>& cfg = {}) {
    //Generate anchor (reference) windows by enumerating aspect ratios X
    //scales wrt a reference (0, 0, 15, 15) window.
    vector<vector<anchor_box>> anchors;
    for (const auto& tmp : cfg) {
        //stride从小到大[32 16 8]
        int bs = tmp.BASE_SIZE;
        vector<float> ratios = tmp.RATIOS;
        vector<int> scales = tmp.SCALES;
        int stride = tmp.STRIDE;

        vector<anchor_box> r = generate_anchors(bs, ratios, scales, stride, dense_anchor);
        anchors.push_back(r);
    }

    return anchors;
}

vector<anchor_box> anchors_plane(int height, int width, int stride, const vector<anchor_box>& base_anchors) {
    /*
    height: height of plane
    width:  width of plane
    stride: stride ot the original image
    anchors_base: a base set of anchors
    */
    vector<anchor_box> all_anchors;
    for(auto & base_anchor : base_anchors) {
        for(int ih = 0; ih < height; ih++) {
            float sh = (float)ih * (float)stride;
            for(int iw = 0; iw < width; iw++) {
                float sw = (float)iw * (float)stride;

                anchor_box tmp{};
                tmp.x1 = base_anchor.x1 + sw;
                tmp.y1 = base_anchor.y1 + sh;
                tmp.x2 = base_anchor.x2 + sw;
                tmp.y2 = base_anchor.y2 + sh;
                all_anchors.push_back(tmp);
            }
        }
    }

    return all_anchors;
}

void clip_boxes(anchor_box &box, int width, int height) {
    //Clip boxes to image boundaries.
    if (box.x1 < 0) {
        box.x1 = 0;
    }
    if (box.y1 < 0) {
        box.y1 = 0;
    }
    if (box.x2 > width - 1) {
        box.x2 = width - 1;
    }
    if (box.y2 > height - 1) {
        box.y2 = height -1;
    }
}


void MNetCov2::nms(std::vector<Anchor>& anchors, float threshold, std::vector<Anchor>& out_anchors) {
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

MNetCov2::MNetCov2(const std::string &model_dir_path) {
    std::vector<int> cpu_devices = {0};
    init(model_dir_path, cpu_devices);
}

MNetCov2::MNetCov2(const std::string &model_dir_path, const std::vector<int> &cpu_devices) {
    init(model_dir_path, cpu_devices);
}

MNetCov2::~MNetCov2() = default;

void MNetCov2::init(const std::string &model_dir_path, const std::vector<int> &cpu_devices) {
    string so_lib_path = model_dir_path + m_model_name  + ".so";
    tvm::runtime::Module mod_syslib = tvm::runtime::Module::LoadFromFile(so_lib_path);

    // json graph
    std::ifstream json_in(model_dir_path + m_model_name + ".json", std::ios::in);
    std::string json_data((std::istreambuf_iterator<char>(json_in)), std::istreambuf_iterator<char>());
    json_in.close();

    // parameters in binary
    std::ifstream params_in(model_dir_path + m_model_name + ".params", std::ios::binary);
    std::string params_data((std::istreambuf_iterator<char>(params_in)), std::istreambuf_iterator<char>());
    params_in.close();

    tvm::runtime::Module mod = (*tvm::runtime::Registry::Get("tvm.graph_executor.create"))(json_data, mod_syslib, m_device_type, m_device_id);

    for (auto& cpu_device : cpu_devices) {
        tvm::runtime::Module mod = (*tvm::runtime::Registry::Get("tvm.graph_executor.create"))(json_data, mod_syslib, m_device_type, cpu_device);
        this->m_handles.emplace_back(std::make_shared<tvm::runtime::Module>(mod));

        TVMByteArray params_arr;
        params_arr.data = params_data.c_str();
        params_arr.size = params_data.length();
        tvm::runtime::PackedFunc load_params = mod.GetFunction("load_params");
        load_params(params_arr);
    }

    vector<float> ratio = {1.0};
    anchor_cfg tmp;
    tmp.SCALES = {32, 16};
    tmp.BASE_SIZE = 16;
    tmp.RATIOS = ratio;
    tmp.ALLOWED_BORDER = 9999;
    tmp.STRIDE = 32;
    m_cfg.push_back(tmp);

    tmp.SCALES = {8, 4};
    tmp.BASE_SIZE = 16;
    tmp.RATIOS = ratio;
    tmp.ALLOWED_BORDER = 9999;
    tmp.STRIDE = 16;
    m_cfg.push_back(tmp);

    tmp.SCALES = {2, 1};
    tmp.BASE_SIZE = 16;
    tmp.RATIOS = ratio;
    tmp.ALLOWED_BORDER = 9999;
    tmp.STRIDE = 8;
    m_cfg.push_back(tmp);

    vector<int> outputW = { 20, 40, 80 };
    vector<int> outputH = { 20, 40, 80 };

    bool dense_anchor = false;
    vector<vector<anchor_box>> anchors_fpn = generate_anchors_fpn(dense_anchor, m_cfg);
    for(size_t i = 0; i < anchors_fpn.size(); i++) {
        string key = "stride" + std::to_string(m_feat_stride_fpn[i]);
        int stride = m_feat_stride_fpn[i];
        m_anchors_fpn[key] = anchors_fpn[i];
        m_num_anchors[key] = anchors_fpn[i].size();
        m_anchors[key] = anchors_plane(outputH[i], outputW[i], stride, m_anchors_fpn[key]);
    }
}

vector<Anchor> MNetCov2::detect(uint8_t* bgr_img, int img_width, int img_height, float score_threshold = 0.5) {
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
        input_data[i] = m_scale * ((float) resized_padding_img[i * 3 + 2] - m_mean);
        input_data[i + m_input_width * m_input_height] = m_scale * ((float) resized_padding_img[i * 3 + 1] - m_mean);
        input_data[i + m_input_width * m_input_height * 2] = m_scale * ((float) resized_padding_img[i * 3] - m_mean);
    }

    auto *mod = (tvm::runtime::Module *)m_handles[m_cur_cpu_device_idx].get();
    if (++m_cur_cpu_device_idx >= m_handles.size()) m_cur_cpu_device_idx = 0;

    tvm::runtime::PackedFunc set_input = mod->GetFunction("set_input");
    tvm::runtime::PackedFunc load_params = mod->GetFunction("load_params");
    tvm::runtime::PackedFunc run = mod->GetFunction("run");
    tvm::runtime::PackedFunc get_output = mod->GetFunction("get_output");

    DLTensor* tvm_input_data;
    int64_t in_shape[4] = { 1, m_input_channel, m_input_height, m_input_width };
    TVMArrayAlloc(in_shape, m_in_ndim, m_dtype_code, m_dtype_bits, m_dtype_lanes, m_device_type, m_device_id, &tvm_input_data);
    TVMArrayCopyFromBytes(tvm_input_data, input_data, m_input_size);

    set_input(m_input_name, tvm_input_data);

    run();

    vector<Anchor> anchors;

    for (int i = 0; i < m_feat_stride_fpn.size(); ++i) {
        int stride = m_feat_stride_fpn[i];

        string key = "stride" + std::to_string(m_feat_stride_fpn[i]);

        tvm::runtime::NDArray tvm_scores_output = get_output(i * m_output_sym);
        tvm::runtime::NDArray tvm_bbox_preds_output = get_output(i * m_output_sym + 1);
        tvm::runtime::NDArray tvm_kps_preds_output = get_output(i * m_output_sym + 2);
        tvm::runtime::NDArray tvm_mask_scores_output = get_output(i * m_output_sym + 3);

        auto scores_output_shape = tvm_scores_output->shape;
        auto bbox_preds_output_shape = tvm_bbox_preds_output->shape;
        auto kps_preds_output_shape = tvm_kps_preds_output->shape;
        auto mask_scores_output_shape = tvm_mask_scores_output->shape;

        // 存储顺序 h * w * num_anchor
        vector<anchor_box> anchor_boxes = m_anchors[key];

        auto scores_output_size = scores_output_shape[0] * scores_output_shape[1] * scores_output_shape[2] * scores_output_shape[3] * sizeof(float);
        auto bbox_preds_output_size = bbox_preds_output_shape[0] * bbox_preds_output_shape[1] * bbox_preds_output_shape[2] * bbox_preds_output_shape[3] * sizeof(float);
        auto kps_preds_output_size = kps_preds_output_shape[0] * kps_preds_output_shape[1] * kps_preds_output_shape[2] * kps_preds_output_shape[3] * sizeof(float);
        auto mask_scores_output_size = mask_scores_output_shape[0] * mask_scores_output_shape[1] * mask_scores_output_shape[2] * mask_scores_output_shape[3] * sizeof(float);

        auto *scores = (float *) malloc(scores_output_size);
        auto *bbox_preds = (float *) malloc(bbox_preds_output_size);
        auto *kps_preds = (float *) malloc(kps_preds_output_size);
        auto *mask_scores = (float *) malloc(mask_scores_output_size);

        tvm_scores_output.CopyToBytes(scores, scores_output_size);
        tvm_bbox_preds_output.CopyToBytes(bbox_preds, bbox_preds_output_size);
        tvm_kps_preds_output.CopyToBytes(kps_preds, kps_preds_output_size);
        tvm_mask_scores_output.CopyToBytes(mask_scores, mask_scores_output_size);

        int count = scores_output_shape[2] * scores_output_shape[3];
        size_t num_anchor = m_num_anchors[key];

        auto _scores = scores + (scores_output_shape[0] * scores_output_shape[1] * scores_output_shape[2] * scores_output_shape[3] / num_anchor);
        auto _mask_scores = mask_scores + (mask_scores_output_shape[0] * mask_scores_output_shape[1] * mask_scores_output_shape[2] * mask_scores_output_shape[3] * num_anchor / 3);

        for (size_t num = 0; num < num_anchor; num++) {
            for (size_t j = 0; j < count; j++) {
                float score = _scores[j + count * num];
                float mask_score = _mask_scores[j + count * num];
                if (score < score_threshold) {
                    continue;
                }

                Anchor anchor;
                anchor.x = bbox_preds[j + count * (0 + num * 4)];
                anchor.y = bbox_preds[j + count * (1 + num * 4)];
                anchor.w = bbox_preds[j + count * (2 + num * 4)];
                anchor.h = bbox_preds[j + count * (3 + num * 4)];

                anchor_box rect = bbox_pred(anchor_boxes[j + count * num], anchor);
                clip_boxes(rect, m_input_width, m_input_height);

                anchor.x = rect.x1;
                anchor.y = rect.y1;
                anchor.w = rect.x2;
                anchor.h = rect.y2;

                for(size_t k = 0; k < 5; k++) {
                    anchor.kps[k * 2] = kps_preds[j + count * (num * 10 + k * 2)] * m_landmark_std;
                    anchor.kps[k * 2 + 1] = kps_preds[j + count * (num * 10 + k * 2 + 1)] * m_landmark_std;
                }

                landmark_pred(anchor_boxes[j + count * num], anchor.kps);

                anchor.score = score;
                anchor.mask_score = mask_score;

                anchors.emplace_back(anchor);
            }
        }

        free(scores);
        free(bbox_preds);
        free(kps_preds);
        free(mask_scores);
    }

    vector<Anchor> nms_anchors;
    nms(anchors, m_nms_thresh, nms_anchors);

    float det_scale = float(new_height) / (float)img_height;
    for (auto& anchor: nms_anchors) {
        anchor.x = anchor.x / det_scale;
        anchor.y = anchor.y / det_scale;
        anchor.w = anchor.w / det_scale - anchor.x;
        anchor.h = anchor.h / det_scale - anchor.y;

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