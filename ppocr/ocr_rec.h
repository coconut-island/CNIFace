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

#include "ocr_cls.h"
#include "utility.h"

namespace PaddleOCR {

    class CRNNRecognizer {
    public:
        explicit CRNNRecognizer(const std::string &model_dir, const std::string &label_path,
                                const int &rec_img_h, const int &rec_img_w,  const std::vector<int> &cpu_devices) {
            this->rec_img_h_ = rec_img_h;
            this->rec_img_w_ = rec_img_w;
            std::vector<int> rec_image_shape = {3, rec_img_h, rec_img_w};
            this->rec_image_shape_ = rec_image_shape;

            this->label_list_ = Utility::ReadDict(label_path);
            this->label_list_.insert(this->label_list_.begin(),
                                     "#"); // blank char for ctc
            this->label_list_.emplace_back(" ");

            LoadModel(model_dir, m_default_model_name, cpu_devices);
        }

        // Load Paddle inference model
        void LoadModel(const std::string &model_dir_path, const std::string &model_name, const std::vector<int> &cpu_devices);

        void Run(std::vector<cv::Mat> img_list, std::vector<std::string> &rec_texts,
                 std::vector<float> &rec_text_scores);

    private:
        std::vector<std::shared_ptr<tvm::runtime::Module>> m_handles;
        std::atomic_int m_cur_cpu_device_idx{0};

        std::string m_default_model_name = "ch_PP-OCRv3_rec_infer";

        int m_dtype_code = kDLFloat;
        int m_dtype_bits = 32;
        int m_dtype_lanes = 1;
        int m_device_type = kDLCPU;
        int m_in_ndim = 4;

        int m_input_channel = 3;
        int m_input_height = 48;
        int m_input_width = 2048;


        std::string m_input_name = "x";

        std::vector<int> m_output_shape = {1, 256, 6625};

        std::vector<std::string> label_list_;

        std::vector<float> mean_ = {0.5f, 0.5f, 0.5f};
        std::vector<float> scale_ = {1 / 0.5f, 1 / 0.5f, 1 / 0.5f};
        bool is_scale_ = true;
        int rec_batch_num_ = 1;
        int rec_img_h_ = 48;
        int rec_img_w_ = 2048;
        std::vector<int> rec_image_shape_ = {3, rec_img_h_, rec_img_w_};
        // pre-process
        CrnnResizeImg resize_op_;
        Normalize normalize_op_;
        PermuteBatch permute_op_;

    }; // class CrnnRecognizer

} // namespace PaddleOCR
