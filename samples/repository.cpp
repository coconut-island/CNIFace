//
// Created by Abel Lee on 2022/6/3.
//

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../retinaface/RetinaFace.h"
#include "../arcface/ArcFace.h"
#include "../repository/Repository.h"
#include "../utils/ImageUtil.h"
#include "../utils/MathUtil.h"
#include "../utils/base64.h"


using namespace std;
using namespace cv;

int main() {
    vector<string> db_names = {"test_db1", "test_db2"};
    float* testFeature;

    Repository::initRepos();

    RetinaFace retinaFace("../models/relay/");
    ArcFace arcFace("../models/relay/");

    for (const auto& db_name : db_names) {
        Repository::addRepo(db_name, arcFace.getFeatureSize());
    }

    cout << "Repository::listRepos()" << endl;
    auto repos = Repository::listRepos();
    for (const auto& repo : repos) {
        cout << "repo name: " << repo->getRepoName() << endl;
    }

    auto img = imread("../images/t1.jpg");

    auto* rgb_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
    ImageUtil::bgr2rgb_packed(img.data, rgb_img, img.cols, img.rows);

    auto anchors = retinaFace.detect(rgb_img, img.cols, img.rows, 0.5);

    int idx = 0;
    for (const auto& anchor : anchors) {
        idx++;
        auto* feature = (float*)malloc(arcFace.getFeatureSize() * sizeof(float));
        arcFace.recognize(rgb_img, img.cols, img.rows, anchor.kps, feature);

        MathUtil::normalize_L2(feature, arcFace.getFeatureSize());

        for (const auto& db_name : db_names) {
            Repository::addWithId(db_name, idx, base64_encode(feature, arcFace.getFeatureSize()));
        }

        if (idx == 1) {
            testFeature = feature;
        } else {
            free(feature);
        }
    }

    for (const auto& db_name : db_names) {
        cout << endl;
        cout << "Start Search DB = " << db_name << endl;
        auto searchResults = Repository::search(db_name, base64_encode(testFeature, arcFace.getFeatureSize()), 10);

        for (const auto& searchResult : searchResults) {
            cout << "searchResult.id = " << to_string(searchResult.id) << endl;
            cout << "searchResult.distance = " << to_string(searchResult.distance) << endl;
        }

        cout << "repo size = " << Repository::size(db_name) << endl;

        Repository::deleteById(db_name, 2);

        searchResults = Repository::search(db_name, base64_encode(testFeature, arcFace.getFeatureSize()), 10);

        for (const auto& searchResult : searchResults) {
            cout << "searchResult.id = " << to_string(searchResult.id) << endl;
            cout << "searchResult.distance = " << to_string(searchResult.distance) << endl;
        }

        cout << "repo size = " << Repository::size(db_name) << endl;
    }

    cout << endl;
    cout << "Repository::destroy(db_names[0])" << endl;
    Repository::destroy(db_names[0]);

    cout << endl;
    cout << "Repository::listRepos()" << endl;
    repos = Repository::listRepos();
    for (const auto& repo : repos) {
        cout << "repo name: " << repo->getRepoName() << endl;
    }

    free(rgb_img);
    free(testFeature);
    return 0;
}
