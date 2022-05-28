//
// Created by Abel Lee on 2022/5/28.
//

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../retinaface/RetinaFace.h"
#include "../utils/ImageUtil.h"

using namespace cv;

void detect_one() {
    auto img = imread("../images/t1.jpg");
    Mat rgb_mat;
    cvtColor(img, rgb_mat, COLOR_BGR2RGB);
    RetinaFace retinaFace("../models/relay/");
    auto* rgb_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
    memcpy(rgb_img, rgb_mat.data, img.rows * img.cols * 3 * sizeof(uint8_t));
    auto anchors = retinaFace.detect(rgb_img, img.cols, img.rows, 0.5);
    free(rgb_img);

    auto draw_img = ImageUtil::draw_faces(img, anchors);
    cv::imshow("draw_img", draw_img);
    cv::waitKey(0);
}


void detect_camera() {
    RetinaFace retinaFace("../models/relay/");

    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "nothing" << endl;
        return;
    }

    int count = 0;
    Mat frame;

    int64_t total = 0;
    int64_t delta_ticks = 0;
    double fps = 0;
    while (count < 1000) {
        count++;
        total++;
        cap >> frame;
        Mat img = frame.clone();

        clock_t current_ticks = clock();
        Mat rgb_mat;
        cvtColor(img, rgb_mat, COLOR_BGR2RGB);
        auto* rgb_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
        memcpy(rgb_img, rgb_mat.data, img.rows * img.cols * 3 * sizeof(uint8_t));
        auto anchors = retinaFace.detect(rgb_img, img.cols, img.rows, 0.5);

        auto draw_img = ImageUtil::draw_faces(img, anchors);

        delta_ticks += (clock() - current_ticks);
        if (delta_ticks > 0) {
            fps = (double )total / ((double)delta_ticks / CLOCKS_PER_SEC);
        }

        cv::putText(draw_img, "FPS = " + std::to_string(fps), Point(30, 30), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 255, 0));
        cv::imshow("draw_img", draw_img);
        cv::waitKey(1);
        free(rgb_img);
    }
}

int main() {
//    detect_one();
    detect_camera();
    return 0;
}