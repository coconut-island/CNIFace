//
// Created by Abel Lee on 2022/5/28.
//

#ifndef CNIFACE_ARCFACE_H
#define CNIFACE_ARCFACE_H

#include <tvm/runtime/module.h>

using namespace std;
using namespace tvm::runtime;

class ArcFace {
public:
    explicit ArcFace(const std::string& model_dir_path);
    ~ArcFace();

    void recognize(uint8_t* rgb_img, int img_width, int img_height, const vector<float>& kps, float* feature);

    int feature_size = 512;

private:
    shared_ptr<Module> handle;

    string model_name = "w600k_r50";

    int dtype_code = kDLFloat;
    int dtype_bits = 32;
    int dtype_lanes = 1;
    int device_type = kDLCPU;
    int device_id = 0;
    int in_ndim = 4;

    float mean = 127.5;
    float scale = 1 / 127.5;

    string input_name = "input.1";

    int input_height = 112;
    int input_width = 112;
    int input_channel = 3;
    size_t input_elements = input_height * input_width * input_channel;
    size_t input_size = input_elements * sizeof(float);

    int64_t out_shapes[2] = {1, 512};

};


#endif //CNIFACE_ARCFACE_H
