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

#include "ocr_cls.h"

#include <tvm/runtime/registry.h>

namespace PaddleOCR {

    void Classifier::Run(std::vector<cv::Mat> img_list,
                         std::vector<int> &cls_labels,
                         std::vector<float> &cls_scores) {

        int img_num = img_list.size();
        std::vector<int> cls_image_shape = {3, 48, 1024};
        for (int beg_img_no = 0; beg_img_no < img_num;
             beg_img_no += this->cls_batch_num_) {
            auto preprocess_start = std::chrono::steady_clock::now();
            int end_img_no = std::min(img_num, beg_img_no + this->cls_batch_num_);
            int batch_num = end_img_no - beg_img_no;
            // preprocess
            std::vector<cv::Mat> norm_img_batch;
            for (int ino = beg_img_no; ino < end_img_no; ino++) {
                cv::Mat srcimg;
                img_list[ino].copyTo(srcimg);
                cv::Mat resize_img;
                this->resize_op_.Run(srcimg, resize_img, false,
                                     cls_image_shape);

                this->normalize_op_.Run(&resize_img, this->mean_, this->scale_,
                                        this->is_scale_);
                if (resize_img.cols < cls_image_shape[2]) {
                    cv::copyMakeBorder(resize_img, resize_img, 0, 0, 0,
                                       cls_image_shape[2] - resize_img.cols,
                                       cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
                }
                norm_img_batch.push_back(resize_img);
            }
            std::vector<float> input(batch_num * cls_image_shape[0] *
                                     cls_image_shape[1] * cls_image_shape[2],
                                     0.0f);
            this->permute_op_.Run(norm_img_batch, input.data());

            // inference.
            int cur_cpu_device_idx = m_cur_cpu_device_idx;
            auto *mod = (tvm::runtime::Module *)m_handles[m_cur_cpu_device_idx].get();
            if (++m_cur_cpu_device_idx >= m_handles.size()) m_cur_cpu_device_idx = 0;

            tvm::runtime::PackedFunc set_input = mod->GetFunction("set_input");
            tvm::runtime::PackedFunc load_params = mod->GetFunction("load_params");
            tvm::runtime::PackedFunc run = mod->GetFunction("run");
            tvm::runtime::PackedFunc get_output = mod->GetFunction("get_output");

            DLTensor* tvm_input_data;
            int64_t in_shape[4] = { batch_num, m_input_channel, m_input_height, m_input_width };
            TVMArrayAlloc(in_shape, m_in_ndim, m_dtype_code, m_dtype_bits, m_dtype_lanes, m_device_type, cur_cpu_device_idx, &tvm_input_data);

            TVMArrayCopyFromBytes(tvm_input_data, input.data(), input.size() * sizeof(float));

            set_input(m_input_name, tvm_input_data);

            run();

            std::vector<float> predict_batch;

            tvm::runtime::NDArray output_t = get_output(0);
            int out_num = std::accumulate(m_output_shape.begin(), m_output_shape.end(), 1,
                                          std::multiplies<int>());

            predict_batch.resize(out_num);
            output_t.CopyToBytes(predict_batch.data(), out_num * sizeof(float));

            auto predict_shape = m_output_shape;

            // postprocess
            auto postprocess_start = std::chrono::steady_clock::now();
            for (int batch_idx = 0; batch_idx < predict_shape[0]; batch_idx++) {
                int label = int(
                        Utility::argmax(&predict_batch[batch_idx * predict_shape[1]],
                                        &predict_batch[(batch_idx + 1) * predict_shape[1]]));
                auto score = float(*std::max_element(
                        &predict_batch[batch_idx * predict_shape[1]],
                        &predict_batch[(batch_idx + 1) * predict_shape[1]]));
                cls_labels[beg_img_no + batch_idx] = label;
                cls_scores[beg_img_no + batch_idx] = score;
            }
        }

    }

    void Classifier::LoadModel(const std::string &model_dir_path, const std::string &model_name, const std::vector<int> &cpu_devices) {
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
} // namespace PaddleOCR
