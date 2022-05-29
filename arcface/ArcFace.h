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

    /**
     * @param input_img 112 * 112 rgb img
     * @param feature 512
     */
    void recognize(uint8_t* rgb_img, float *feature);

    /**
     * @param rgb_img source img
     * @param img_width source img width
     * @param img_height source img height
     * @param kps anchor.kps
     * @param feature 512
     */
    void recognize(uint8_t* rgb_img, int img_width, int img_height, const vector<float>& kps, float* feature);

    size_t getInputWidth() const;
    size_t getInputHeight() const;
    size_t getInputElements() const;
    size_t getFeatureSize() const;

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

    int feature_size = 512;

    float coordinates_112_112[10] = {38.2946, 73.5318, 56.0252, 41.5493, 70.7299,
                     51.6963, 51.5014, 71.7366, 92.3655, 92.2041};
};


#endif //CNIFACE_ARCFACE_H
