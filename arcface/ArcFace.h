//
// Created by Abel Lee on 2022/5/28.
//

#ifndef CNIFACE_ARCFACE_H
#define CNIFACE_ARCFACE_H

#include <tvm/runtime/module.h>

using namespace std;

class ArcFace {
public:
    explicit ArcFace(const std::string &model_dir_path, const std::vector<int> &cpu_devices);
    explicit ArcFace(const std::string &model_dir_path, const std::string &model_name, const std::vector<int> &cpu_devices);
    ~ArcFace();

    /**
     * @param input_img 112 * 112 bgr img
     * @param feature 512
     */
    void recognize(const uint8_t* img, float *feature);

    /**
     * @param bgr_img source img
     * @param img_width source img width
     * @param img_height source img height
     * @param kps anchor.kps
     * @param feature 512
     */
    void recognize(const uint8_t* bgr_img, int img_width, int img_height, const vector<float>& kps, float* feature);

    size_t getFeatureSize() const;

private:
    void init(const std::string &model_dir_path, const std::string &model_name, const std::vector<int> &cpu_devices);

private:
    std::vector<shared_ptr<tvm::runtime::Module>> m_handles;
    std::atomic_int m_cur_cpu_device_idx{0};

    string m_default_model_name = "w600k_r50";

    int m_dtype_code = kDLFloat;
    int m_dtype_bits = 32;
    int m_dtype_lanes = 1;
    int m_device_type = kDLCPU;
    int m_device_id = 0;
    int m_in_ndim = 4;

    float m_mean = 127.5;
    float m_scale = 1 / 127.5;

    string m_input_name = "input.1";

    int m_input_height = 112;
    int m_input_width = 112;
    int m_input_channel = 3;
    size_t m_input_elements = m_input_height * m_input_width * m_input_channel;
    size_t m_input_size = m_input_elements * sizeof(float);

    int64_t m_out_shapes[2] = {1, 512};

    int m_feature_size = 512;

    float m_coordinates_112_112[10] = {38.2946, 73.5318, 56.0252, 41.5493, 70.7299,
                     51.6963, 51.5014, 71.7366, 92.3655, 92.2041};
};


#endif //CNIFACE_ARCFACE_H
