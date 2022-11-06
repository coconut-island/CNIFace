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

#include "postprocess_op.h"
#include "preprocess_op.h"

namespace PaddleOCR {

    class DBDetector {
    public:
        explicit DBDetector(const std::string &model_dir, const double &det_db_thresh,
                            const double &det_db_box_thresh,
                            const double &det_db_unclip_ratio,
                            const std::string &det_db_score_mode,
                            const bool &use_dilation, const std::vector<int> &cpu_devices) {

            this->det_db_thresh_ = det_db_thresh;
            this->det_db_box_thresh_ = det_db_box_thresh;
            this->det_db_unclip_ratio_ = det_db_unclip_ratio;
            this->det_db_score_mode_ = det_db_score_mode;
            this->use_dilation_ = use_dilation;

            LoadModel(model_dir, m_default_model_name, cpu_devices);
        }

        // Load Paddle inference model
        void LoadModel(const std::string &model_dir_path, const std::string &model_name, const std::vector<int> &cpu_devices);

        // Run predictor
        void Run(cv::Mat &img, std::vector<std::vector<std::vector<int>>> &boxes);

    private:
        std::vector<std::shared_ptr<tvm::runtime::Module>> m_handles;
        std::atomic_int m_cur_cpu_device_idx{0};

        std::string m_default_model_name = "ch_PP-OCRv3_det_infer";

        int m_dtype_code = kDLFloat;
        int m_dtype_bits = 32;
        int m_dtype_lanes = 1;
        int m_device_type = kDLCPU;
        int m_in_ndim = 4;

        int m_input_channel = 3;
        int m_input_height = 640;
        int m_input_width = 640;

        std::string m_input_name = "x";

        std::vector<int> m_output_shape = {1, 1, 640, 640};

        int limit_side_len_ = 640;

        double det_db_thresh_ = 0.3;
        double det_db_box_thresh_ = 0.5;
        double det_db_unclip_ratio_ = 2.0;
        std::string det_db_score_mode_ = "slow";
        bool use_dilation_ = false;

        std::vector<float> mean_ = {0.485f, 0.456f, 0.406f};
        std::vector<float> scale_ = {1 / 0.229f, 1 / 0.224f, 1 / 0.225f};
        bool is_scale_ = true;

        // pre-process
        ResizeImgType0 resize_op_;
        Normalize normalize_op_;
        Permute permute_op_;

        // post-process
        DBPostProcessor post_processor_;
    };

} // namespace PaddleOCR