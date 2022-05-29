//
// Created by Abel Lee on 2022/5/28.
//

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../retinaface/RetinaFace.h"
#include "../arcface/ArcFace.h"
#include "../utils/ImageUtil.h"
#include "../utils/MathUtil.h"
#include "../utils/CPUTimer.h"

using namespace cv;

int main() {
    RetinaFace retinaFace("../models/relay/");
    ArcFace arcFace("../models/relay/");

    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "nothing" << endl;
        return 1;
    }

    size_t count = 1000;
    size_t arc_count = 0;

    float* golden_feature;

    Mat frame;

    CPUTimer cpuTimer;

    CPUTimer arcFaceCpuTimer("ArcFace");
    while (count--) {
        count++;
        cap >> frame;
        Mat img = frame.clone();

        cpuTimer.start();
        auto* rgb_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
        ImageUtil::bgr2rgb_packed(img.data, rgb_img, img.cols, img.rows);

        auto anchors = retinaFace.detect(rgb_img, img.cols, img.rows, 0.5);

        for (const auto& anchor : anchors) {
            arc_count++;

            arcFaceCpuTimer.start();
            auto* feature = (float*)malloc(arcFace.feature_size * sizeof(float));
            arcFace.recognize(rgb_img, img.cols, img.rows, anchor.kps, feature);
            arcFaceCpuTimer.stop();

            if (arc_count <= 1) {
                golden_feature = feature;
                continue;
            }

            auto similarity = MathUtil::cosine_similarity(feature, golden_feature, arcFace.feature_size);

            cv::putText(img, "similarity = " + std::to_string(similarity), Point(anchor.x, anchor.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 255, 0));

            free(feature);
        }

        cpuTimer.stop();

        arcFaceCpuTimer.print();

        free(rgb_img);
        ImageUtil::draw_faces(img, anchors);

        cv::putText(img, "FPS = " + std::to_string(cpuTimer.getFPS()), Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 255, 0));
        cv::imshow("draw_img", img);
        cv::waitKey(1);
    }

    free(golden_feature);
}