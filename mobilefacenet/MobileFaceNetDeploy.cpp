//
// Created by Abel Lee on 2022/5/15.
//

#include "MobileFaceNetDeploy.h"
#include <tvm/runtime/registry.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <memory>

using namespace std;
using namespace tvm::runtime;

MobileFaceNetDeploy::MobileFaceNetDeploy(const std::string& model_dir_path) {
    string so_lib_path = model_dir_path + model_prefix  + ".so";
    Module mod_syslib = Module::LoadFromFile(so_lib_path);

    // json graph
    std::ifstream json_in(model_dir_path + model_prefix + ".json", std::ios::in);
    std::string json_data((std::istreambuf_iterator<char>(json_in)), std::istreambuf_iterator<char>());
    json_in.close();

    // parameters in binary
    std::ifstream params_in(model_dir_path + model_prefix + ".params", std::ios::binary);
    std::string params_data((std::istreambuf_iterator<char>(params_in)), std::istreambuf_iterator<char>());
    params_in.close();

    tvm::runtime::Module mod = (*Registry::Get("tvm.graph_executor.create"))(json_data, mod_syslib, device_type, device_id);

    this->handle = std::make_unique<tvm::runtime::Module>(mod);

    TVMByteArray params_arr;
    params_arr.data = params_data.c_str();
    params_arr.size = params_data.length();
    tvm::runtime::PackedFunc load_params = mod.GetFunction("load_params");
    load_params(params_arr);
}

MobileFaceNetDeploy::~MobileFaceNetDeploy() = default;

cv::Mat MobileFaceNetDeploy::forward(const cv::Mat& inputImageAligned) {
    //mobilefacnet preprocess has been written in graph.
    cv::Mat tensor = cv::dnn::blobFromImage(inputImageAligned, 1.0, cv::Size(in_w, in_h), cv::Scalar(0, 0, 0), true);
    //convert uint8 to float32 and convert to RGB via opencv dnn function
    DLTensor *input;
    const int64_t in_shape[4] = {1, in_c, in_w, in_h};
    TVMArrayAlloc(in_shape, in_ndim, dtype_code, dtype_bits, dtype_lanes, device_type, device_id, &input);
    TVMArrayCopyFromBytes(input, tensor.data, total_input * sizeof(float));
    auto *mod = (tvm::runtime::Module *) handle.get();
    tvm::runtime::PackedFunc set_input = mod->GetFunction("set_input");
    tvm::runtime::PackedFunc run = mod->GetFunction("run");
    tvm::runtime::PackedFunc get_output = mod->GetFunction("get_output");
    set_input("data", input);
    run();
    tvm::runtime::NDArray res = get_output(0);
    cv::Mat vector(128, 1, CV_32F);
    memcpy(vector.data, res->data, 128 * sizeof(float));
    cv::Mat _l2;
    cv::multiply(vector, vector, _l2);
    float l2 = (float)cv::sqrt(cv::sum(_l2).val[0]);
    vector = vector / l2;
    TVMArrayFree(input);
    return vector;
}

