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

#pragma once

#include <tvm/runtime/module.h>

#include "preprocess_op.h"
#include "utility.h"

namespace PaddleOCR {

    class Classifier {
    public:
        explicit Classifier(const std::string &model_dir, const double &cls_thresh, const int &cls_batch_num, const std::vector<int> &cpu_devices) {

            this->cls_thresh = cls_thresh;
            this->cls_batch_num_ = cls_batch_num;

            LoadModel(model_dir, m_default_model_name, cpu_devices);
        }
        double cls_thresh = 0.9;

        // Load Paddle inference model
        void LoadModel(const std::string &model_dir_path, const std::string &model_name, const std::vector<int> &cpu_devices);

        void Run(std::vector<cv::Mat> img_list, std::vector<int> &cls_labels,
                 std::vector<float> &cls_scores);

    private:
        std::vector<std::shared_ptr<tvm::runtime::Module>> m_handles;
        std::atomic_int m_cur_cpu_device_idx{0};

        std::string m_default_model_name = "ch_ppocr_mobile_v2.0_cls_infer";

        int m_dtype_code = kDLFloat;
        int m_dtype_bits = 32;
        int m_dtype_lanes = 1;
        int m_device_type = kDLCPU;
        int m_in_ndim = 4;

        int m_input_channel = 3;
        int m_input_height = 48;
        int m_input_width = 1024;

        std::string m_input_name = "x";

        std::vector<int> m_output_shape = {1, 2};


        std::vector<float> mean_ = {0.5f, 0.5f, 0.5f};
        std::vector<float> scale_ = {1 / 0.5f, 1 / 0.5f, 1 / 0.5f};
        bool is_scale_ = true;
        int cls_batch_num_ = 1;
        // pre-process
        ClsResizeImg resize_op_;
        Normalize normalize_op_;
        PermuteBatch permute_op_;

    }; // class Classifier

} // namespace PaddleOCR
