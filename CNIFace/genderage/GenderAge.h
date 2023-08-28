//
// Created by Abel Lee on 2022/8/25.
//

#ifndef CNIFACE_GENDERAGE_H
#define CNIFACE_GENDERAGE_H

#include <tvm/runtime/module.h>
#include <atomic>

using namespace std;

class GenderAgeResult {
public:
    int gender;
    int age;
};

class GenderAge {
public:
    explicit GenderAge(const std::string &model_dir_path);
    explicit GenderAge(const std::string &model_dir_path, const std::vector<int> &cpu_devices);
    ~GenderAge();

    GenderAgeResult infer(const uint8_t* bgr_img, int img_width, int img_height, const vector<float>& kps);

    GenderAgeResult infer(const uint8_t* bgr_img);

private:
    void init(const std::string &model_dir_path, const std::string &model_name, const std::vector<int> &cpu_devices);

private:
    std::vector<shared_ptr<tvm::runtime::Module>> m_handles;
    std::atomic_int m_cur_cpu_device_idx{0};

    string m_default_model_name = "genderage";

    int m_dtype_code = kDLFloat;
    int m_dtype_bits = 32;
    int m_dtype_lanes = 1;
    int m_device_type = kDLCPU;
    int m_in_ndim = 4;

    float m_mean = 0.0;
    float m_scale = 1.0;

    string m_input_name = "data";

    int m_input_height = 96;
    int m_input_width = 96;
    int m_input_channel = 3;
    size_t m_input_elements = m_input_height * m_input_width * m_input_channel;
    size_t m_input_size = m_input_elements * sizeof(float);

    int64_t m_out_shapes[2] = {1, 3};

    float m_coordinates_96_96[10] = {30.2946, 65.5318, 48.0252, 33.5493, 62.7299,
                                       43.6963, 43.5014, 63.7366, 84.3655, 84.2041};

};


#endif //CNIFACE_GENDERAGE_H
