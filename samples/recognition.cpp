//
// Created by Abel Lee on 2022/5/28.
//

#include <opencv2/highgui.hpp>

#include "../retinaface/RetinaFace.h"
#include "../arcface/ArcFace.h"
#include "../utils/MathUtil.h"
#include "../utils/ImageUtil.h"
#include "../utils/CPUTimer.h"

using namespace cv;

int main() {
    auto img = imread("../images/t1.jpg");
    auto* rgb_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
    ImageUtil::bgr2rgb_packed(img.data, rgb_img, img.cols, img.rows);

    CPUTimer cpuTimer;

    RetinaFace retinaFace("../models/relay/");
    ArcFace arcFace("../models/relay/");

    auto anchors = retinaFace.detect(rgb_img, img.cols, img.rows, 0.5);

    for (const auto& anchor : anchors) {
        cpuTimer.start();
        auto* feature = (float*)malloc(arcFace.getFeatureSize() * sizeof(float));
        arcFace.recognize(rgb_img, img.cols, img.rows, anchor.kps, feature);
        cpuTimer.stop();

        auto similarity = MathUtil::cosine_similarity(feature, feature, arcFace.getFeatureSize());

        std::cout << "feature = " << std::to_string(similarity) << std::endl;
        free(feature);
    }

    cpuTimer.print();
    free(rgb_img);
}