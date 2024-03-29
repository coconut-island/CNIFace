//
// Created by Abel Lee on 2022/5/28.
//

#include <opencv2/highgui.hpp>

#include "../retinaface/RetinaFace.h"
#include "../utils/ImageUtil.h"
#include "../utils/CVUtil.h"
#include "../utils/CPUTimer.h"

using namespace cv;

int main() {
    RetinaFace retinaFace("../models/relay/");
    CPUTimer cpuTimer;

    auto img = imread("../images/t1.jpg");

    cpuTimer.start();
    auto* bgr_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
    memcpy(bgr_img, img.data, img.rows * img.cols * 3 * sizeof(uint8_t));

    auto anchors = retinaFace.detect(bgr_img, img.cols, img.rows, 0.5);
    cpuTimer.stop();
    cpuTimer.print();

    free(bgr_img);

    CVUtil::draw_faces(img, anchors);
    cv::imwrite("./t1_detected.jpg", img);
    return 0;
}