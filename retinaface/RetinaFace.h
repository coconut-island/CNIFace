//
// Created by Abel Lee on 2022/5/28.
//

#ifndef CNIFACE_RETINAFACE_H
#define CNIFACE_RETINAFACE_H

#include <string>
#include <vector>

#include <tvm/runtime/module.h>

#include "../common/Anchor.h"

using namespace std;
using namespace tvm::runtime;

class RetinaFace {
public:
    explicit RetinaFace(const std::string& model_dir_path);
    ~RetinaFace();
    vector<Anchor> detect(uint8_t* rgb_img, int img_width, int img_height, float score_threshold);
    static void nms(std::vector<Anchor>& anchors, float threshold, std::vector<Anchor>& out_anchors);
private:
    shared_ptr<Module> handle;

    string model_name = "det_10g";
    float det_thresh = 0.5;
    float nms_thresh = 0.4;

    int dtype_code = kDLFloat;
    int dtype_bits = 32;
    int dtype_lanes = 1;
    int device_type = kDLCPU;
    int device_id = 0;
    int in_ndim = 4;

    float mean = 127.5;
    float scale = 1 / 128.0;
    int num_anchors = 2;

    string input_name = "input.1";
    vector<string> output_names = {"448", "471", "494", "451", "474", "497", "454", "477", "500"};
    int64_t out_shapes[9][2] = {
            {12800, 1}, {3200, 1}, {800, 1},
            {12800, 4}, {3200, 4}, {800, 4},
            {12800, 10}, {3200, 10}, {800, 10}
    };
    int fmc = 3;
    vector<int> feat_stride_fpn = { 8, 16, 32 };

    int input_height = 640;
    int input_width = 640;
    int input_channel = 3;
    size_t input_elements = input_height * input_width * input_channel;
    size_t input_size = input_elements * sizeof(float);
};


#endif //CNIFACE_RETINAFACE_H
