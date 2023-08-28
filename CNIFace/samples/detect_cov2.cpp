//
// Created by Abel Lee on 2022/5/28.
//

#include <opencv2/highgui.hpp>

#include "../mnet_cov2/MNetCov2.h"
#include "../utils/ImageUtil.h"
#include "../utils/CVUtil.h"
#include "../utils/CPUTimer.h"

using namespace cv;

int main() {
    MNetCov2 mNetCov2("../models/relay/");
    CPUTimer cpuTimer;

    auto img = imread("../images/t1.jpg");

    cpuTimer.start();
    auto* rgb_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
    memcpy(rgb_img, img.data, img.rows * img.cols * 3 * sizeof(uint8_t));

    auto anchors = mNetCov2.detect(rgb_img, img.cols, img.rows, 0.8);
    cpuTimer.stop();
    cpuTimer.print();

    free(rgb_img);

    CVUtil::draw_faces(img, anchors);
    cv::imwrite("./t1_detected.jpg", img);
    return 0;
}