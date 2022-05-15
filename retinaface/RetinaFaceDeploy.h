//
// Created by Abel Lee on 2022/5/11.
//

#ifndef MAIN_RETINAFACEDEPLOY_H
#define MAIN_RETINAFACEDEPLOY_H

#include <string>
#include <tvm/runtime/module.h>
#include <opencv2/highgui/highgui.hpp>
#include "anchor_generator.h"
#include "../common.h"


struct RetinaOutput {
    std::vector<Anchor> result;
    cv::Point2f ratio;
};

class RetinaFaceDeploy {
private:
    std::unique_ptr<tvm::runtime::Module> handle;

    std::string model_prefix = "mnet.25.x86.cpu";
    int dtype_code = kDLFloat;
    int dtype_bits = 32;
    int dtype_lanes = 1;
    int device_type = kDLCPU;
    int device_id = 0;

    int in_c = 3;
    int in_h = 480;
    int in_w = 640;
    int total_input = in_c * in_w * in_h;
    int in_ndim = 4;
public:
    explicit RetinaFaceDeploy(const std::string& model_dir_path);
    ~RetinaFaceDeploy();
    RetinaOutput forward(const cv::Mat& image);
};


#endif //MAIN_RETINAFACEDEPLOY_H
