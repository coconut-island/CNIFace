// Copyright (c) 2020 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "ocr_det.h"

#include <tvm/runtime/registry.h>

namespace PaddleOCR {

    void DBDetector::LoadModel(const std::string &model_dir_path, const std::string &model_name, const std::vector<int> &cpu_devices) {
        std::string so_lib_path = model_dir_path + model_name  + ".so";
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

    void DBDetector::Run(cv::Mat &img, std::vector<std::vector<std::vector<int>>> &boxes) {
    float ratio_h{};
    float ratio_w{};

    cv::Mat srcimg;
    cv::Mat resize_img;
    img.copyTo(srcimg);

    auto preprocess_start = std::chrono::steady_clock::now();
    this->resize_op_.Run(img, resize_img, this->limit_side_len_, ratio_h, ratio_w);

    this->normalize_op_.Run(&resize_img, this->mean_, this->scale_, this->is_scale_);

    std::vector<float> input(1 * 3 * resize_img.rows * resize_img.cols, 0.0f);
    this->permute_op_.Run(&resize_img, input.data());
    auto preprocess_end = std::chrono::steady_clock::now();

    // Inference.
    int cur_cpu_device_idx = m_cur_cpu_device_idx;
    auto *mod = (tvm::runtime::Module *)m_handles[m_cur_cpu_device_idx].get();
    if (++m_cur_cpu_device_idx >= m_handles.size()) m_cur_cpu_device_idx = 0;

    tvm::runtime::PackedFunc set_input = mod->GetFunction("set_input");
    tvm::runtime::PackedFunc load_params = mod->GetFunction("load_params");
    tvm::runtime::PackedFunc run = mod->GetFunction("run");
    tvm::runtime::PackedFunc get_output = mod->GetFunction("get_output");

    DLTensor* tvm_input_data;
    int64_t in_shape[4] = { 1, m_input_channel, m_input_height, m_input_width };
    TVMArrayAlloc(in_shape, m_in_ndim, m_dtype_code, m_dtype_bits, m_dtype_lanes, m_device_type, cur_cpu_device_idx, &tvm_input_data);
    TVMArrayCopyFromBytes(tvm_input_data, input.data(), input.size() * sizeof(float));

    set_input(m_input_name, tvm_input_data);

    run();

    std::vector<float> out_data;

    tvm::runtime::NDArray output_t = get_output(0);
    int out_num = std::accumulate(m_output_shape.begin(), m_output_shape.end(), 1,
                                  std::multiplies<int>());

    out_data.resize(out_num);
    output_t.CopyToBytes(out_data.data(), out_num * sizeof(float));
    auto inference_end = std::chrono::steady_clock::now();

    auto postprocess_start = std::chrono::steady_clock::now();
    int n2 = m_output_shape[2];
    int n3 = m_output_shape[3];
    int n = n2 * n3;

    std::vector<float> pred(n, 0.0);
    std::vector<unsigned char> cbuf(n, ' ');

    for (int i = 0; i < n; i++) {
    pred[i] = float(out_data[i]);
    cbuf[i] = (unsigned char)((out_data[i]) * 255);
    }

    cv::Mat cbuf_map(n2, n3, CV_8UC1, (unsigned char *)cbuf.data());
    cv::Mat pred_map(n2, n3, CV_32F, (float *)pred.data());

    const double threshold = this->det_db_thresh_ * 255;
    const double maxvalue = 255;
    cv::Mat bit_map;
    cv::threshold(cbuf_map, bit_map, threshold, maxvalue, cv::THRESH_BINARY);
    if (this->use_dilation_) {
    cv::Mat dila_ele =
            cv::getStructuringElement(cv::MORPH_RECT, cv::Size(2, 2));
    cv::dilate(bit_map, bit_map, dila_ele);
    }

    boxes = post_processor_.BoxesFromBitmap(
            pred_map, bit_map, this->det_db_box_thresh_, this->det_db_unclip_ratio_,
            this->det_db_score_mode_);

    boxes = post_processor_.FilterTagDetRes(boxes, ratio_h, ratio_w, srcimg);
}

} // namespace PaddleOCR
