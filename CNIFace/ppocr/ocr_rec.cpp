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

#include "ocr_rec.h"

#include <tvm/runtime/registry.h>

namespace PaddleOCR {

    void CRNNRecognizer::Run(std::vector<cv::Mat> img_list,
                             std::vector<std::string> &rec_texts,
                             std::vector<float> &rec_text_scores) {
        int img_num = img_list.size();
        std::vector<float> width_list;
        for (int i = 0; i < img_num; i++) {
            width_list.push_back(float(img_list[i].cols) / img_list[i].rows);
        }
        std::vector<int> indices = Utility::argsort(width_list);

        for (int beg_img_no = 0; beg_img_no < img_num;
             beg_img_no += this->rec_batch_num_) {
            int end_img_no = std::min(img_num, beg_img_no + this->rec_batch_num_);
            int batch_num = end_img_no - beg_img_no;
            int imgH = this->rec_image_shape_[1];
            int imgW = this->rec_image_shape_[2];
            float max_wh_ratio = imgW * 1.0 / imgH;
            for (int ino = beg_img_no; ino < end_img_no; ino++) {
                int h = img_list[indices[ino]].rows;
                int w = img_list[indices[ino]].cols;
                float wh_ratio = w * 1.0 / h;
                max_wh_ratio = std::max(max_wh_ratio, wh_ratio);
            }

            int batch_width = imgW;
            std::vector<cv::Mat> norm_img_batch;
            for (int ino = beg_img_no; ino < end_img_no; ino++) {
                cv::Mat srcimg;
                img_list[indices[ino]].copyTo(srcimg);
                cv::Mat resize_img;
                this->resize_op_.Run(srcimg, resize_img, max_wh_ratio,
                                     false, this->rec_image_shape_);
                this->normalize_op_.Run(&resize_img, this->mean_, this->scale_,
                                        this->is_scale_);
                norm_img_batch.push_back(resize_img);
                batch_width = std::max(resize_img.cols, batch_width);
            }

            std::vector<float> input(batch_num * 3 * imgH * batch_width, 0.0f);
            this->permute_op_.Run(norm_img_batch, input.data());
            // Inference.
            int cur_cpu_device_idx = m_cur_cpu_device_idx;
            auto *mod = (tvm::runtime::Module *)m_handles[m_cur_cpu_device_idx].get();
            if (++m_cur_cpu_device_idx >= m_handles.size()) m_cur_cpu_device_idx = 0;

            tvm::runtime::PackedFunc set_input = mod->GetFunction("set_input");
            tvm::runtime::PackedFunc load_params = mod->GetFunction("load_params");
            tvm::runtime::PackedFunc run = mod->GetFunction("run");
            tvm::runtime::PackedFunc get_output = mod->GetFunction("get_output");

            std::vector<float> predict_batch;
            auto predict_shape = std::vector<int> {batch_num, m_output_shape[1], m_output_shape[2]};

            int out_num = std::accumulate(predict_shape.begin(), predict_shape.end(), 1,
                                          std::multiplies<int>());

            predict_batch.resize(out_num);

            DLTensor* tvm_input_data;
            int64_t in_shape[4] = { 1, m_input_channel, m_input_height, m_input_width };

            TVMArrayAlloc(in_shape, m_in_ndim, m_dtype_code, m_dtype_bits, m_dtype_lanes, m_device_type, cur_cpu_device_idx, &tvm_input_data);

            TVMArrayCopyFromBytes(tvm_input_data, input.data(), input.size() * sizeof(float));

            set_input(m_input_name, tvm_input_data);

            run();

            TVMArrayFree(tvm_input_data);

            tvm::runtime::NDArray output_t = get_output(0);

            output_t.CopyToBytes(predict_batch.data(), m_output_shape[1] * m_output_shape[2] * sizeof(float));

            for (int m = 0; m < predict_shape[0]; m++) {
                std::string str_res;
                int argmax_idx;
                int last_index = 0;
                float score = 0.f;
                int count = 0;
                float max_value = 0.0f;

                for (int n = 0; n < predict_shape[1]; n++) {
                    // get idx
                    argmax_idx = int(Utility::argmax(
                            &predict_batch[(m * predict_shape[1] + n) * predict_shape[2]],
                            &predict_batch[(m * predict_shape[1] + n + 1) * predict_shape[2]]));
                    // get score
                    max_value = float(*std::max_element(
                            &predict_batch[(m * predict_shape[1] + n) * predict_shape[2]],
                            &predict_batch[(m * predict_shape[1] + n + 1) * predict_shape[2]]));

                    if (argmax_idx > 0 && (!(n > 0 && argmax_idx == last_index))) {
                        score += max_value;
                        count += 1;
                        str_res += label_list_[argmax_idx];
                    }
                    last_index = argmax_idx;
                }
                score /= count;
                if (std::isnan(score)) {
                    continue;
                }
                rec_texts[indices[beg_img_no + m]] = str_res;
                rec_text_scores[indices[beg_img_no + m]] = score;
            }
        }
    }


    void CRNNRecognizer::LoadModel(const std::string &model_dir_path, const std::string &model_name, const std::vector<int> &cpu_devices) {
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



