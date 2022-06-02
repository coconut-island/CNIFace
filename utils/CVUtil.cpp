//
// Created by Abel Lee on 2022/6/2.
//

#include "CVUtil.h"


void CVUtil::draw_faces(cv::Mat& img, const vector<Anchor>& anchors) {
    auto red_color = cv::Scalar(0, 0, 255);
    auto green_color = cv::Scalar(0, 255, 0);

    for (const auto& anchor : anchors) {
        cv::rectangle(img, cv::Point(anchor.x, anchor.y),cv::Point(anchor.w, anchor.h), red_color, 2);
        for (int i = 0; i < anchor.kps.size(); i += 2) {
            cv::circle(img, cv::Point(anchor.kps[i], anchor.kps[i + 1]), 3, green_color, cv::FILLED, cv::LINE_AA);
        }
    }
}
