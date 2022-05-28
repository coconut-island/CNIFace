//
// Created by Abel Lee on 2022/5/28.
//

#include <fstream>

#include <tvm/runtime/registry.h>

#include "ArcFace.h"
#include "../utils/ImageUtil.h"

ArcFace::ArcFace(const string &model_dir_path) {
    string so_lib_path = model_dir_path + model_name  + ".so";
    Module mod_syslib = Module::LoadFromFile(so_lib_path);

    // json graph
    std::ifstream json_in(model_dir_path + model_name + ".json", std::ios::in);
    std::string json_data((std::istreambuf_iterator<char>(json_in)), std::istreambuf_iterator<char>());
    json_in.close();

    // parameters in binary
    std::ifstream params_in(model_dir_path + model_name + ".params", std::ios::binary);
    std::string params_data((std::istreambuf_iterator<char>(params_in)), std::istreambuf_iterator<char>());
    params_in.close();

    tvm::runtime::Module mod = (*Registry::Get("tvm.graph_executor.create"))(json_data, mod_syslib, device_type, device_id);

    this->handle = std::make_shared<tvm::runtime::Module>(mod);

    TVMByteArray params_arr;
    params_arr.data = params_data.c_str();
    params_arr.size = params_data.length();
    tvm::runtime::PackedFunc load_params = mod.GetFunction("load_params");
    load_params(params_arr);
}

ArcFace::~ArcFace() = default;

void ArcFace::recognize(uint8_t* bgr_img, int img_width, int img_height, const vector<float>& kps, float *feature) {
    float dst[10] = {38.2946, 73.5318, 56.0252, 41.5493, 70.7299,
                     51.6963, 51.5014, 71.7366, 92.3655, 92.2041};

    float src[10];
    for (int i = 0; i < 5; i++) {
        src[i] = kps[2 * i];
        src[i + 5] = kps[2 * i + 1];
    }

    float M[6];
    ImageUtil::getAffineMatrix(src, dst, M);
    auto* input_img = (uint8_t*)malloc(input_elements * sizeof(uint8_t));
    ImageUtil::warpAffineMatrix(bgr_img, input_img, img_width, img_height, input_width, input_height, M);

    auto* input_data = (float*)malloc(input_size);
    for (int i = 0; i < input_width * input_height; i++) {
        input_data[i] = scale * ((float)input_img[i * 3] - mean);
        input_data[i + input_width * input_height] = scale * ((float)input_img[i * 3 + 1] - mean);
        input_data[i + input_width * input_height * 2] = scale * ((float)input_img[i * 3 + 2] - mean);
    }

    auto *mod = (Module *)handle.get();

    PackedFunc set_input = mod->GetFunction("set_input");
    PackedFunc load_params = mod->GetFunction("load_params");
    PackedFunc run = mod->GetFunction("run");
    PackedFunc get_output = mod->GetFunction("get_output");

    DLTensor* tvm_input_data;
    int64_t in_shape[4] = { 1, input_channel, input_height, input_width };
    TVMArrayAlloc(in_shape, in_ndim, dtype_code, dtype_bits, dtype_lanes, device_type, device_id, &tvm_input_data);
    TVMArrayCopyFromBytes(tvm_input_data, input_data, input_size);

    set_input(input_name, tvm_input_data);

    run();

    tvm::runtime::NDArray tvm_output_data = get_output(0);

    memcpy(feature, tvm_output_data->data, out_shapes[0] * out_shapes[1] * sizeof(float));

    free(input_img);
}


