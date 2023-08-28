//
// Created by Abel Lee on 2022/8/28.
//

#include <opencv2/highgui.hpp>

#include "../retinaface/RetinaFace.h"
#include "../genderage/GenderAge.h"
#include "../utils/ImageUtil.h"
#include "../utils/CPUTimer.h"


using namespace cv;

int main() {
    auto img = imread("/Users/abel/image/2.jpg");
    auto* bgr_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
    memcpy(bgr_img, img.data, img.rows * img.cols * 3 * sizeof(uint8_t));

    CPUTimer cpuTimer;

    RetinaFace retinaFace("../models/relay/");
    GenderAge genderAge("../models/relay/");

    auto anchors = retinaFace.detect(bgr_img, img.cols, img.rows, 0.5);

    int idx = 0;
    for (const auto& anchor : anchors) {
        cpuTimer.start();
        auto result = genderAge.infer(bgr_img, img.cols, img.rows, anchor.kps);
        cpuTimer.stop();

        std::string sex = result.gender == 1 ? "Male" : "Female";
        std::cout << "idx=" << idx << std::endl;
        std::cout << "gender=" << sex << std::endl;
        std::cout << "age=" << result.age << std::endl;

        idx++;
    }

    cpuTimer.print();
    free(bgr_img);
    return 0;
}