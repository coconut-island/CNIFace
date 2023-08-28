//
// Created by Abel Lee on 2022/5/29.
//

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../mnet_cov2/MNetCov2.h"
#include "../utils/ImageUtil.h"
#include "../utils/CVUtil.h"
#include "../utils/CPUTimer.h"

using namespace cv;

int main() {
    MNetCov2 mNetCov2("../models/relay/");

    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "nothing" << endl;
        return 1;
    }

    size_t count = 1000;
    Mat frame;

    CPUTimer cpuTimer;
    while (count--) {
        count++;
        cap >> frame;
        Mat img = frame.clone();

        cpuTimer.start();

        auto* bgr_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
        memcpy(bgr_img, img.data, img.rows * img.cols * 3 * sizeof(uint8_t));

        auto anchors = mNetCov2.detect(bgr_img, img.cols, img.rows, 0.5);

        cpuTimer.stop();

        free(bgr_img);
        CVUtil::draw_faces(img, anchors);

        cv::putText(img, "FPS = " + std::to_string(cpuTimer.getFPS()), Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 255, 0));
        cv::imshow("draw_img", img);
        cv::waitKey(1);
    }
    return 0;
}