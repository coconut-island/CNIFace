//
// Created by Abel Lee on 2022/5/15.
//

#ifndef MAIN_MOBILEFACENETDEPLOY_H
#define MAIN_MOBILEFACENETDEPLOY_H

#include <tvm/runtime/module.h>
#include <opencv2/highgui/highgui.hpp>

class MobileFaceNetDeploy {
private:
    std::unique_ptr<tvm::runtime::Module> handle;

public:
    explicit MobileFaceNetDeploy(const std::string& model_dir_path);
    ~MobileFaceNetDeploy();
    cv::Mat forward(const cv::Mat& inputImageAligned);

    std::string model_prefix = "mobilefacenet.x86.cpu";
    int dtype_code = kDLFloat;
    int dtype_bits = 32;
    int dtype_lanes = 1;
    int device_type = kDLCPU;
    int device_id = 0;
    const int in_ndim = 4;

    int in_c = 3;
    int in_h = 112;
    int in_w = 112;
    int total_input = in_c * in_w * in_h;
};


#endif //MAIN_MOBILEFACENETDEPLOY_H
