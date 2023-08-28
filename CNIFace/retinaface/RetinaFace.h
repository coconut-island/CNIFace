//
// Created by Abel Lee on 2022/5/28.
//

#ifndef CNIFACE_RETINAFACE_H
#define CNIFACE_RETINAFACE_H

#include <string>
#include <vector>
#include <atomic>

#include <tvm/runtime/module.h>

#include "../common/Anchor.h"

using namespace std;

class RetinaFace {
public:
    explicit RetinaFace(const std::string& model_dir_path);
    explicit RetinaFace(const std::string& model_dir_path, const std::vector<int> &cpu_devices);
    explicit RetinaFace(const std::string& model_dir_path, const std::string& model_name, const std::vector<int> &cpu_devices);
    ~RetinaFace();
    vector<Anchor> detect(uint8_t* rgb_img, int img_width, int img_height, float score_threshold);
    static void nms(std::vector<Anchor>& anchors, float threshold, std::vector<Anchor>& out_anchors);

private:
    void init(const std::string& model_dir_path, const std::string& model_name, const std::vector<int> &cpu_devices);

private:
    std::vector<shared_ptr<tvm::runtime::Module>> m_handles;
    std::atomic_int m_cur_cpu_device_idx{0};

    string m_default_model_name = "det_10g";
    float m_nms_thresh = 0.4;

    int m_dtype_code = kDLFloat;
    int m_dtype_bits = 32;
    int m_dtype_lanes = 1;
    int m_device_type = kDLCPU;
    int m_in_ndim = 4;

    float m_mean = 127.5;
    float m_scale = 1 / 128.0;
    int m_num_anchors = 2;

    string m_input_name = "input.1";
    int64_t m_out_shapes[9][2] = {
            {12800, 1}, {3200, 1}, {800, 1},
            {12800, 4}, {3200, 4}, {800, 4},
            {12800, 10}, {3200, 10}, {800, 10}
    };

    int m_output_sym = 3;
    vector<int> m_feat_stride_fpn = { 8, 16, 32 };

    int m_input_height = 640;
    int m_input_width = 640;
    int m_input_channel = 3;
    size_t m_input_elements = m_input_height * m_input_width * m_input_channel;
    size_t m_input_size = m_input_elements * sizeof(float);
};


#endif //CNIFACE_RETINAFACE_H
