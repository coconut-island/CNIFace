//
// Created by Abel Lee on 2022/9/18.
//

#include <string>
#include <opencv2/highgui.hpp>

#include "../repository/Repository.h"
#include "../retinaface/RetinaFace.h"
#include "../arcface/ArcFace.h"
#include "../utils/MathUtil.h"
#include "../utils/ImageUtil.h"


using namespace cv;


int main() {
    string testRepoName = "test_repo";

    Repository repositoryService;

    if (repositoryService.existRepository(testRepoName)) {
        repositoryService.deleteRepository(testRepoName);
    }

    repositoryService.createRepository(testRepoName);

    auto img = imread("../images/t1.jpg");
    auto* rgb_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
    ImageUtil::bgr2rgb_packed(img.data, rgb_img, img.cols, img.rows);
    float* goldenFeature;

    RetinaFace retinaFace("../models/relay/");
    ArcFace arcFace("../models/relay/");

    auto anchors = retinaFace.detect(rgb_img, img.cols, img.rows, 0.5);

    int idx = 0;
    for (const auto& anchor : anchors) {
        auto* feature = (float*)malloc(arcFace.getFeatureSize() * sizeof(float));
        arcFace.recognize(rgb_img, img.cols, img.rows, anchor.kps, feature);

        MathUtil::normalize_L2(feature, arcFace.getFeatureSize());

        if (idx == 0) {
            goldenFeature = feature;
        }

        repositoryService.addOrUpdateItem(testRepoName, idx, feature);
        auto similarity = MathUtil::cosine_similarity(goldenFeature, feature, arcFace.getFeatureSize());

        std::cout << "feature similarity = " << std::to_string(similarity) << std::endl;

        if (idx != 0) {
            free(feature);
        }
        idx++;
    }

    free(rgb_img);

    auto searchResults = repositoryService.search(testRepoName, 10, goldenFeature);

    LOG(INFO) << "searchResults size = " << searchResults.size();
    for (auto &searchResult : searchResults) {
        LOG(INFO) << "result id = "<< searchResult.id << ", result score = " << searchResult.score;
    }


    free(goldenFeature);
    return 0;
}