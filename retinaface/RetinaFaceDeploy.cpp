//
// Created by Abel Lee on 2022/5/11.
//

#include "RetinaFaceDeploy.h"

#include <tvm/runtime/registry.h>
#include <fstream>
#include <memory>
#include "tools.h"

using namespace std;
using namespace tvm::runtime;


RetinaFaceDeploy::RetinaFaceDeploy(const string& model_dir_path) {
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

RetinaOutput RetinaFaceDeploy::forward(const cv::Mat& image) {
    if(!image.data) {
        cerr << "load img error!" << endl;

    }
    DLTensor* x;
    int64_t in_shape[4] = { 1, in_c, in_h, in_w };
    TVMArrayAlloc(in_shape, in_ndim, dtype_code, dtype_bits, dtype_lanes, device_type, device_id, &x);

    int w1 = ceil(in_w / 32.0);
    int w2 = ceil(in_w / 16.0);
    int w3 = ceil(in_w / 8.0);
    int h1 = ceil(in_h / 32.0);
    int h2 = ceil(in_h / 16.0);
    int h3 = ceil(in_h / 8.0);

    auto *mod = (tvm::runtime::Module *)handle.get();

    tvm::runtime::PackedFunc set_input = mod->GetFunction("set_input");
    tvm::runtime::PackedFunc load_params = mod->GetFunction("load_params");
    tvm::runtime::PackedFunc run = mod->GetFunction("run");
    tvm::runtime::PackedFunc get_output = mod->GetFunction("get_output");

    auto* data_x = (float*)malloc(total_input * sizeof(float));

    cv::Mat resizeImage;
    cv::resize(image, resizeImage, cv::Size(in_w, in_h), cv::INTER_AREA);
    cv::Mat input_mat;

    resizeImage.convertTo(input_mat, CV_32FC3);

    cv::Mat split_mat[3];
    cv::split(input_mat, split_mat);
    memcpy(data_x, split_mat[2].ptr<float>(), input_mat.cols * input_mat.rows * sizeof(float));
    memcpy(data_x + input_mat.cols * input_mat.rows, split_mat[1].ptr<float>(), input_mat.cols * input_mat.rows * sizeof(float));
    memcpy(data_x + input_mat.cols * input_mat.rows * 2, split_mat[0].ptr<float>(), input_mat.cols * input_mat.rows * sizeof(float));
    TVMArrayCopyFromBytes(x, data_x, total_input * sizeof(float));

    set_input("data", x);

    run();

    std::vector<AnchorGenerator> ac(_feat_stride_fpn.size());
    for (int i = 0; i < _feat_stride_fpn.size(); ++i) {
        int stride = _feat_stride_fpn[i];
        ac[i].Init(stride, anchor_cfg[stride], false);
    }

    std::vector<Anchor> proposals;
    proposals.clear();

    int64_t w[3] = {w1, w2, w3};
    int64_t h[3] = {h1, h2, h3};
    int64_t out_size[9] = {w1 * h1 * 4, w1 * h1 * 8, w1 * h1 * 20, w2 * h2 * 4, w2 * h2 * 8, w2 * h2 * 20,
                           w3 * h3 * 4, w3 * h3 * 8, w3 * h3 * 20};

    int out_ndim = 4;
    int64_t out_shape[9][4] = {{1, 4,  h1, w1},
                               {1, 8,  h1, w1},
                               {1, 20, h1, w1},
                               {1, 4,  h2, w2},
                               {1, 8,  h2, w2},
                               {1, 20, h2, w2},
                               {1, 4,  h3, w3},
                               {1, 8,  h3, w3},
                               {1, 20, h3, w3}};

    DLTensor* y[9];
    for (int i = 0 ; i < 9; i++)
        TVMArrayAlloc(out_shape[i], out_ndim, dtype_code, dtype_bits, dtype_lanes, device_type, device_id, &y[i]);

    for (int i = 0 ; i < 9; i+=3) {
        get_output(i, y[i]);
        get_output(i + 1, y[i + 1]);
        get_output(i + 2, y[i + 2]);

        ulsMatF clsMat(w[i / 3], h[i / 3], 4);
        ulsMatF regMat(w[i / 3], h[i / 3], 8);
        ulsMatF ptsMat(w[i / 3], h[i / 3], 20);

        TVMArrayCopyToBytes(y[i], clsMat.m_data, out_size[i] * sizeof(float));
        TVMArrayCopyToBytes(y[i + 1], regMat.m_data, out_size[i + 1] * sizeof(float));
        TVMArrayCopyToBytes(y[i + 2], ptsMat.m_data, out_size[i + 2] * sizeof(float));

        ac[i / 3].FilterAnchor(clsMat, regMat, ptsMat, proposals);
    }

    // nms
    std::vector<Anchor> result;
    nms_cpu(proposals, nms_threshold, result);

    free(data_x);
    TVMArrayFree(x);
    for (auto & i : y) {
        TVMArrayFree(i);
    }


    float ratio_x = (float)image.cols / (float)in_w;
    float ratio_y = (float)image.rows / (float)in_h;

    RetinaOutput retinaOutput;
    retinaOutput.result = result;
    retinaOutput.ratio.x = ratio_x;
    retinaOutput.ratio.y = ratio_y;
    return retinaOutput;
}



RetinaFaceDeploy::~RetinaFaceDeploy() = default;
