//
// Created by Abel Lee on 2022/7/24.
//

#ifndef CNIFACE_MNETCOV2_H
#define CNIFACE_MNETCOV2_H

#include <string>
#include <vector>
#include <unordered_map>

#include <tvm/runtime/module.h>

#include "../common/Anchor.h"

using namespace std;

struct anchor_box {
    float x1;
    float y1;
    float x2;
    float y2;
};

struct anchor_cfg {
public:
    int STRIDE;
    vector<int> SCALES;
    int BASE_SIZE;
    vector<float> RATIOS;
    int ALLOWED_BORDER;

    anchor_cfg() {
        STRIDE = 0;
        SCALES.clear();
        BASE_SIZE = 0;
        RATIOS.clear();
        ALLOWED_BORDER = 0;
    }
};

class MNetCov2 {
public:
    explicit MNetCov2(const std::string& model_dir_path, const std::vector<int> &cpu_devices);
    ~MNetCov2();
    vector<Anchor> detect(uint8_t* rgb_img, int img_width, int img_height, float score_threshold);
    static void nms(std::vector<Anchor>& anchors, float threshold, std::vector<Anchor>& out_anchors);
private:
    std::vector<shared_ptr<tvm::runtime::Module>> m_handles;
    std::atomic_int m_cur_cpu_device_idx{0};

    string m_model_name = "mnet_cov2";
    float m_nms_thresh = 0.4;

    int m_dtype_code = kDLFloat;
    int m_dtype_bits = 32;
    int m_dtype_lanes = 1;
    int m_device_type = kDLCPU;
    int m_device_id = 0;
    int m_in_ndim = 4;

    float m_mean = 0.0;
    float m_scale = 1.0;

    string m_input_name = "data";

    int m_output_sym = 4;

    vector<int> m_feat_stride_fpn = { 32, 16, 8 };

    int m_input_height = 640;
    int m_input_width = 640;
    int m_input_channel = 3;
    size_t m_input_elements = m_input_height * m_input_width * m_input_channel;
    size_t m_input_size = m_input_elements * sizeof(float);


    float m_landmark_std = 0.2f;

    vector<anchor_cfg> m_cfg;

    map<string, vector<anchor_box>> m_anchors_fpn;

    map<string, vector<anchor_box>> m_anchors;

    map<string, int> m_num_anchors;
};





#endif //CNIFACE_MNETCOV2_H
