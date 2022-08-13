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
using namespace tvm::runtime;

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
    explicit MNetCov2(const std::string& model_dir_path);
    ~MNetCov2();
    vector<Anchor> detect(uint8_t* rgb_img, int img_width, int img_height, float score_threshold);
    static void nms(std::vector<Anchor>& anchors, float threshold, std::vector<Anchor>& out_anchors);
private:
    shared_ptr<Module> handle;

    string model_name = "mnet_cov2";
    float nms_thresh = 0.4;

    int dtype_code = kDLFloat;
    int dtype_bits = 32;
    int dtype_lanes = 1;
    int device_type = kDLCPU;
    int device_id = 0;
    int in_ndim = 4;

    float mean = 0.0;
    float scale = 1.0;
    float std = 1.0;
    int num_anchors = 2;

    string input_name = "data";
    vector<string> output_names = {"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11"};

    int output_sym = 4;

    vector<int> feat_stride_fpn = { 32, 16, 8 };

    int input_height = 640;
    int input_width = 640;
    int input_channel = 3;
    size_t input_elements = input_height * input_width * input_channel;
    size_t input_size = input_elements * sizeof(float);


    float landmark_std = 0.2f;

    vector<float> _ratio;
    vector<anchor_cfg> cfg;

    //每一层fpn的anchor形状
    map<string, vector<anchor_box>> _anchors_fpn;
    //每一层所有点的anchor
    map<string, vector<anchor_box>> _anchors;
    //每一层fpn有几种形状的anchor
    //也就是ratio个数乘以scales个数
    map<string, int> _num_anchors;
};





#endif //CNIFACE_MNETCOV2_H
