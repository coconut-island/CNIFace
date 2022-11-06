

#include "../ppocr/ocr_det.h"
#include "../ppocr/ocr_cls.h"
#include "../ppocr/ocr_rec.h"



int main() {
    auto *detector = new PaddleOCR::DBDetector(
            "../models/relay/", 0.3, 0.6,
            1.5, "slow", false, std::vector<int>{0});

    auto *classifier = new PaddleOCR::Classifier("../models/relay/", 0.9, 1, std::vector<int>{0});

    auto *recognizer = new PaddleOCR::CRNNRecognizer("../models/relay/",
            "../resources/ppocr_keys_v1.txt", 48, 2048, std::vector<int>{0});

    std::vector<std::vector<std::vector<int>>> boxes;

    std::vector<PaddleOCR::OCRPredictResult> ocr_results;

    cv::Mat img = cv::imread("../images/ocr_2_180.jpg");

    detector->Run(img, boxes);

    for (auto & boxe : boxes) {
        PaddleOCR::OCRPredictResult res;
        res.box = boxe;
        ocr_results.push_back(res);
    }

    PaddleOCR::Utility::sorted_boxes(ocr_results);

    std::vector<cv::Mat> img_list;
    for (auto & ocr_result : ocr_results) {
        cv::Mat crop_img;
        crop_img = PaddleOCR::Utility::GetRotateCropImage(img, ocr_result.box);
        img_list.push_back(crop_img);
    }

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

    std::vector<std::string> rec_texts(img_list.size(), "");
    std::vector<float> rec_text_scores(img_list.size(), 0);
    recognizer->Run(img_list, rec_texts, rec_text_scores);
    for (int i = 0; i < rec_texts.size(); i++) {
        ocr_results[i].text = rec_texts[i];
        ocr_results[i].score = rec_text_scores[i];
    }

//    PaddleOCR::Utility::VisualizeBboxes(img, ocr_results,
//                               "../images/ocr_out_1.jpg");

    delete detector;
    delete classifier;
    delete recognizer;
    return 0;
}