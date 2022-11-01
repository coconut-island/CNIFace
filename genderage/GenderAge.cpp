//
// Created by Abel Lee on 2022/8/25.
//
#include "GenderAge.h"
#include <fstream>

#include <tvm/runtime/registry.h>

#include "../utils/ImageUtil.h"

GenderAge::GenderAge(const string &model_dir_path) {
    std::vector<int> cpu_devices = {0};
    init(model_dir_path, m_default_model_name, cpu_devices);
}

GenderAge::GenderAge(const string &model_dir_path, const std::vector<int> &cpu_devices) {
    init(model_dir_path, m_default_model_name, cpu_devices);
}

GenderAge::~GenderAge() = default;

void GenderAge::init(const string &model_dir_path, const string &model_name, const std::vector<int> &cpu_devices) {
    string so_lib_path = model_dir_path + model_name  + ".so";
    tvm::runtime::Module mod_syslib = tvm::runtime::Module::LoadFromFile(so_lib_path);

    // json graph
    std::ifstream json_in(model_dir_path + model_name + ".json", std::ios::in);
    std::string json_data((std::istreambuf_iterator<char>(json_in)), std::istreambuf_iterator<char>());
    json_in.close();

    // parameters in binary
    std::ifstream params_in(model_dir_path + model_name + ".params", std::ios::binary);
    std::string params_data((std::istreambuf_iterator<char>(params_in)), std::istreambuf_iterator<char>());
    params_in.close();

    for (auto& cpu_device : cpu_devices) {
        tvm::runtime::Module mod = (*tvm::runtime::Registry::Get("tvm.graph_executor.create"))(json_data, mod_syslib, m_device_type, cpu_device);
        this->m_handles.emplace_back(std::make_shared<tvm::runtime::Module>(mod));

        TVMByteArray params_arr;
        params_arr.data = params_data.c_str();
        params_arr.size = params_data.length();
        tvm::runtime::PackedFunc load_params = mod.GetFunction("load_params");
        load_params(params_arr);
    }
}

GenderAgeResult GenderAge::infer(const uint8_t *bgr_img) {
    auto* input_data = (float*)malloc(m_input_size);
    for (int i = 0; i < m_input_width * m_input_height; i++) {
        input_data[i] = m_scale * ((float)bgr_img[i * 3 + 2] - m_mean);
        input_data[i + m_input_width * m_input_height] = m_scale * ((float)bgr_img[i * 3 + 1] - m_mean);
        input_data[i + m_input_width * m_input_height * 2] = m_scale * ((float)bgr_img[i * 3] - m_mean);
    }

    auto *mod = (tvm::runtime::Module *)m_handles[m_cur_cpu_device_idx].get();
    if (++m_cur_cpu_device_idx >= m_handles.size()) m_cur_cpu_device_idx = 0;

    tvm::runtime::PackedFunc set_input = mod->GetFunction("set_input");
    tvm::runtime::PackedFunc load_params = mod->GetFunction("load_params");
    tvm::runtime::PackedFunc run = mod->GetFunction("run");
    tvm::runtime::PackedFunc get_output = mod->GetFunction("get_output");

    DLTensor* tvm_input_data;
    int64_t in_shape[4] = { 1, m_input_channel, m_input_height, m_input_width };
    TVMArrayAlloc(in_shape, m_in_ndim, m_dtype_code, m_dtype_bits, m_dtype_lanes, m_device_type, m_device_id, &tvm_input_data);
    TVMArrayCopyFromBytes(tvm_input_data, input_data, m_input_size);

    set_input(m_input_name, tvm_input_data);

    run();

    tvm::runtime::NDArray tvm_output_data = get_output(0);

    auto output_data = (float*)malloc(m_out_shapes[0] * m_out_shapes[1] * sizeof(float));
    memcpy(output_data, tvm_output_data->data, m_out_shapes[0] * m_out_shapes[1] * sizeof(float));

    GenderAgeResult genderAgeResult{};
    genderAgeResult.gender = output_data[0] > output_data[1] ? 0 : 1;
    genderAgeResult.age = int(std::round(output_data[1] * 100.0f));
    free(output_data);
    TVMArrayFree(tvm_input_data);
    free(input_data);

    return genderAgeResult;
}

GenderAgeResult GenderAge::infer(const uint8_t *bgr_img, int img_width, int img_height, const vector<float>& kps) {
    assert(kps.size() == 10);

    auto dst = m_coordinates_96_96;

    float src[10];
    for (int i = 0; i < 5; i++) {
        src[i] = kps[2 * i];
        src[i + 5] = kps[2 * i + 1];
    }

    float M[6];
    ImageUtil::getAffineMatrix(src, dst, M);
    auto* input_img = (uint8_t*)malloc(m_input_elements * sizeof(uint8_t));
    ImageUtil::warpAffineMatrix(bgr_img, input_img, img_width, img_height, m_input_width, m_input_height, M);

    auto result = infer(input_img);
    free(input_img);

    return result;
}