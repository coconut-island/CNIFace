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

//feature similarity = 1.000000
//feature similarity = 0.062621
//feature similarity = -0.010281
//feature similarity = -0.081489
//feature similarity = 0.038155
//feature similarity = -0.030517

int main() {
    auto img = imread("../images/t1.jpg");
    auto* rgb_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
    ImageUtil::bgr2rgb_packed(img.data, rgb_img, img.cols, img.rows);
    float* goldenFeature;

    CPUTimer cpuTimer;

    RetinaFace retinaFace("../models/relay/");
    ArcFace arcFace("../models/relay/");

    auto anchors = retinaFace.detect(rgb_img, img.cols, img.rows, 0.5);

    int idx = 0;
    for (const auto& anchor : anchors) {
        cpuTimer.start();
        auto* feature = (float*)malloc(arcFace.getFeatureSize() * sizeof(float));
        arcFace.recognize(rgb_img, img.cols, img.rows, anchor.kps, feature);
        cpuTimer.stop();

        MathUtil::normalize_L2(feature, arcFace.getFeatureSize());

        if (idx == 0) {
            goldenFeature = feature;
        }
        auto similarity = MathUtil::cosine_similarity(goldenFeature, feature, arcFace.getFeatureSize());

        std::cout << "feature similarity = " << std::to_string(similarity) << std::endl;

        if (idx != 0) {
            free(feature);
        }
        idx++;
    }

    cpuTimer.print();
    free(rgb_img);
    free(goldenFeature);
    return 0;
}