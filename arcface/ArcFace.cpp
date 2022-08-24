//
// Created by Abel Lee on 2022/5/28.
//

#include <fstream>

#include <tvm/runtime/registry.h>

#include "ArcFace.h"
#include "../utils/ImageUtil.h"

ArcFace::ArcFace(const string &model_dir_path) {
    init(model_dir_path, m_default_model_name);
}

ArcFace::ArcFace(const string &model_dir_path, const string &model_name) {
    init(model_dir_path, model_name);
}

ArcFace::~ArcFace() = default;

void ArcFace::init(const std::string &model_dir_path, const std::string &model_name) {
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

    tvm::runtime::Module mod = (*Registry::Get("tvm.graph_executor.create"))(json_data, mod_syslib, m_device_type, m_device_id);

    this->m_handle = std::make_shared<tvm::runtime::Module>(mod);

    TVMByteArray params_arr;
    params_arr.data = params_data.c_str();
    params_arr.size = params_data.length();
    tvm::runtime::PackedFunc load_params = mod.GetFunction("load_params");
    load_params(params_arr);
}

void ArcFace::recognize(const uint8_t* rgb_img, float *feature) {
    auto* input_data = (float*)malloc(m_input_size);
    for (int i = 0; i < m_input_width * m_input_height; i++) {
        input_data[i] = m_scale * ((float)rgb_img[i * 3] - m_mean);
        input_data[i + m_input_width * m_input_height] = m_scale * ((float)rgb_img[i * 3 + 1] - m_mean);
        input_data[i + m_input_width * m_input_height * 2] = m_scale * ((float)rgb_img[i * 3 + 2] - m_mean);
    }

    auto *mod = (Module *)m_handle.get();

    PackedFunc set_input = mod->GetFunction("set_input");
    PackedFunc load_params = mod->GetFunction("load_params");
    PackedFunc run = mod->GetFunction("run");
    PackedFunc get_output = mod->GetFunction("get_output");

    DLTensor* tvm_input_data;
    int64_t in_shape[4] = { 1, m_input_channel, m_input_height, m_input_width };
    TVMArrayAlloc(in_shape, m_in_ndim, m_dtype_code, m_dtype_bits, m_dtype_lanes, m_device_type, m_device_id, &tvm_input_data);
    TVMArrayCopyFromBytes(tvm_input_data, input_data, m_input_size);

    set_input(m_input_name, tvm_input_data);

    run();

    tvm::runtime::NDArray tvm_output_data = get_output(0);

    memcpy(feature, tvm_output_data->data, m_out_shapes[0] * m_out_shapes[1] * sizeof(float));

    TVMArrayFree(tvm_input_data);
    free(input_data);
}

void ArcFace::recognize(uint8_t* rgb_img, int img_width, int img_height, const vector<float>& kps, float *feature) {
    assert(kps.size() == 10);

    auto dst = m_coordinates_112_112;

    float src[10];
    for (int i = 0; i < 5; i++) {
        src[i] = kps[2 * i];
        src[i + 5] = kps[2 * i + 1];
    }

    float M[6];
    ImageUtil::getAffineMatrix(src, dst, M);
    auto* input_img = (uint8_t*)malloc(m_input_elements * sizeof(uint8_t));
    ImageUtil::warpAffineMatrix(rgb_img, input_img, img_width, img_height, m_input_width, m_input_height, M);

    recognize(input_img, feature);
    free(input_img);
}

size_t ArcFace::getFeatureSize() const {
    return m_feature_size;
}


