//
// Created by Abel Lee on 2022/5/29.
//

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../retinaface/RetinaFace.h"
#include "../utils/ImageUtil.h"

using namespace cv;

int main() {
    RetinaFace retinaFace("../models/relay/");

    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "nothing" << endl;
        return 1;
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

        auto* rgb_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
        ImageUtil::bgr2rgb_packed(img.data, rgb_img, img.cols, img.rows);

        auto anchors = retinaFace.detect(rgb_img, img.cols, img.rows, 0.5);

        free(rgb_img);
        auto draw_img = ImageUtil::draw_faces(img, anchors);

        delta_ticks += (clock() - current_ticks);
        if (delta_ticks > 0) {
            fps = (double )total / ((double)delta_ticks / CLOCKS_PER_SEC);
        }

        cv::putText(draw_img, "FPS = " + std::to_string(fps), Point(30, 30), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 255, 0));
        cv::imshow("draw_img", draw_img);
        cv::waitKey(1);
    }
    return 0;
}