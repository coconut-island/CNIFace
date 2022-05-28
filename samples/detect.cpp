//
// Created by Abel Lee on 2022/5/28.
//

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../retinaface/RetinaFace.h"
#include "../utils/ImageUtil.h"

using namespace cv;

int main() {
    RetinaFace retinaFace("../models/relay/");

    auto img = imread("../images/t1.jpg");

    auto* rgb_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
    ImageUtil::bgr2rgb_packed(img.data, rgb_img, img.cols, img.rows);

    auto anchors = retinaFace.detect(rgb_img, img.cols, img.rows, 0.5);

    free(rgb_img);

    auto draw_img = ImageUtil::draw_faces(img, anchors);
    cv::imshow("draw_img", draw_img);
    cv::waitKey(0);
    return 0;
}