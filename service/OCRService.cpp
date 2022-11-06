//
// Created by Abel Lee on 2022/11/6.
//

#include "OCRService.h"
#include "../utils/base64.h"


OCRService::OCRService(const std::string &model_dir, const std::string &resource_dir,
                       const std::vector<int> &cpu_devices) {
    detector = new PaddleOCR::DBDetector(
            model_dir, 0.3, 0.6,
            1.5, "slow", false, cpu_devices);

    classifier = new PaddleOCR::Classifier(model_dir, 0.9, 1, cpu_devices);

    recognizer = new PaddleOCR::CRNNRecognizer(model_dir, resource_dir + "/ppocr_keys_v1.txt", 48, 2048, cpu_devices);

}

OCRService::~OCRService() {
    delete detector;
    delete classifier;
    delete recognizer;
}

grpc::Status OCRService::ocr(::grpc::ServerContext *context, const ::cniface::OCRRequest *request, ::cniface::OCRResponse *response) {
    response->set_code(0);
    response->set_message("OK");

    const auto& faceImageBase64 = request->imagebase64();
    auto imgStr = base64_decode(faceImageBase64);
    std::vector<char> base64_img(imgStr.begin(), imgStr.end());
    auto img = cv::imdecode(base64_img, cv::IMREAD_UNCHANGED);

    std::vector<std::vector<std::vector<int>>> boxes;

    std::vector<PaddleOCR::OCRPredictResult> ocr_results;

    detector->Run(img, boxes);

    for (auto & boxe : boxes) {
        PaddleOCR::OCRPredictResult res;
        res.box = boxe;
        ocr_results.push_back(res);
    }

    PaddleOCR::Utility::sorted_boxes(ocr_results);

    std::vector<cv::Mat> img_list;

    if (request->iscls() || request->isrec()) {
        for (auto & ocr_result : ocr_results) {
            cv::Mat crop_img;
            crop_img = PaddleOCR::Utility::GetRotateCropImage(img, ocr_result.box);
            img_list.push_back(crop_img);
        }
    }

    if (request->iscls()) {
        std::vector<int> cls_labels(img_list.size(), 0);
        std::vector<float> cls_scores(img_list.size(), 0);

        classifier->Run(img_list, cls_labels, cls_scores);

        for (int i = 0; i < cls_labels.size(); i++) {
            ocr_results[i].cls_label = cls_labels[i];
            ocr_results[i].cls_score = cls_scores[i];
        }

        for (int i = 0; i < img_list.size(); i++) {
            if (ocr_results[i].cls_label % 2 == 1 &&
                ocr_results[i].cls_score > classifier->cls_thresh) {
                cv::rotate(img_list[i], img_list[i], 1);
            }
        }
    }

    if (request->isrec()) {
        std::vector<std::string> rec_texts(img_list.size(), "");
        std::vector<float> rec_text_scores(img_list.size(), 0);
        recognizer->Run(img_list, rec_texts, rec_text_scores);
        for (int i = 0; i < rec_texts.size(); i++) {
            ocr_results[i].text = rec_texts[i];
            ocr_results[i].score = rec_text_scores[i];
        }
    }


    for (const auto& ocr_result : ocr_results) {
        auto result = response->add_results();
        for (auto &item : ocr_result.box) {
            for (auto &po : item) {
                result->add_position((float)po);
            }
        }
        result->set_score(ocr_result.score);
        result->set_text(ocr_result.text);
        result->set_clsscore(ocr_result.cls_score);
        result->set_clslabel(ocr_result.cls_label);
    }

    return grpc::Status::OK;
}

