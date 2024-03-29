//
// Created by Abel Lee on 2022/6/2.
//

#include "CVUtil.h"


void CVUtil::draw_faces(cv::Mat& img, const vector<Anchor>& anchors) {
    auto red_color = cv::Scalar(0, 0, 255);
    auto green_color = cv::Scalar(0, 255, 0);

    for (const auto& anchor : anchors) {
        cv::putText(img, "score = " + std::to_string(anchor.score), cv::Point(anchor.x, anchor.y - 5), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 255, 0));
        cv::rectangle(img, cv::Point(anchor.x, anchor.y), cv::Point(anchor.x + anchor.w, anchor.y + anchor.h), anchor.mask_score > 0.2f ? red_color : green_color , 2);
        for (int i = 0; i < anchor.kps.size(); i += 2) {
            cv::circle(img, cv::Point(anchor.kps[i], anchor.kps[i + 1]), 3, green_color, cv::FILLED, cv::LINE_AA);
        }
    }
}
