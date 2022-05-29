//
// Created by Abel Lee on 2022/5/29.
//

#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <unordered_map>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <faiss/IndexFlat.h>
#include <faiss/IndexIVFFlat.h>
#include <faiss/MetricType.h>
#include <faiss/utils/distances.h>

#include "../retinaface/RetinaFace.h"
#include "../arcface/ArcFace.h"
#include "../utils/ImageUtil.h"
#include "../utils/CPUTimer.h"

using namespace std;
using namespace cv;
using namespace faiss;

using idx_t = faiss::Index::idx_t;

int main() {
    string filePath = "/Users/abel/Downloads/faces_112_less";
    vector<string> names;
    DIR *pDir;
    struct dirent* ptr;
    if(!(pDir = opendir(filePath.c_str()))){
        cout<<"Folder doesn't Exist!"<<endl;
        return 1;
    }
    while((ptr = readdir(pDir)) != nullptr) {
        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0 && strcmp(ptr->d_name, ".DS_Store") != 0){
            names.emplace_back(ptr->d_name);
            cout << ptr->d_name << endl;
        }
    }
    closedir(pDir);

    vector<string> face_file_paths;
    unordered_map<string, string> face_file_paths_name_map;
    for (const string& name : names) {
        DIR *_pDir;
        struct dirent* _ptr;
        string file_dir_path = filePath + "/" + name;
        if(!(_pDir = opendir(file_dir_path.c_str()))){
            cout<<"Folder doesn't Exist!"<<endl;
            return 1;
        }
        while ((_ptr = readdir(_pDir)) != nullptr){
            if (strcmp(_ptr->d_name, ".") != 0 && strcmp(_ptr->d_name, "..") != 0 && strcmp(_ptr->d_name, ".DS_Store") != 0){
                cout << file_dir_path + "/" + _ptr->d_name << endl;
                face_file_paths.emplace_back(file_dir_path + "/" + _ptr->d_name);
                face_file_paths_name_map[file_dir_path + "/" + _ptr->d_name] = name;
            }
        }
        closedir(_pDir);
    }

    RetinaFace retinaFace("../models/relay/");
    ArcFace arcFace("../models/relay/");

    int d = arcFace.getFeatureSize();
    int nlist = 1;

    IndexFlat quantizer(d, faiss::METRIC_INNER_PRODUCT);
    IndexIVFFlat index(&quantizer, d, nlist, faiss::METRIC_INNER_PRODUCT);

    unordered_map<idx_t, string> id_name_map;
    vector<float*> features;

    cout << "files size: " << face_file_paths.size() << endl;
    for (idx_t j = 0; j < face_file_paths.size(); ++j) {
        cout << "loaded file : " << j << endl;
        string& face_file_path = face_file_paths[j];
        auto img = imread(face_file_path);
        auto* resized_img = (uint8_t*)malloc(arcFace.getInputElements() * sizeof(uint8_t));
        ImageUtil::bilinear_resize(img.data, resized_img, img.cols, img.rows, arcFace.getInputWidth(), arcFace.getInputHeight());

        auto* rgb_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
        memcpy(rgb_img, resized_img, img.rows * img.cols * 3 * sizeof(uint8_t));

        auto* feature = (float*)malloc(arcFace.getFeatureSize() * sizeof(float));
        arcFace.recognize(rgb_img, feature);

        features.emplace_back(feature);

//        if (index.ntotal >= 50) {
//            index.nlist = 4;
//        }
        index.train(1, feature);
        index.add_with_ids(1, feature, &j);

        string name = face_file_paths_name_map[face_file_path];
        id_name_map[j] = name;

        free(resized_img);
        free(rgb_img);
    }


    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "nothing" << endl;
        return -1;
    }

    CPUTimer cpuTimer;

    CPUTimer retrievalCpuTimer("Retrieval");

    size_t count = 1000;
    Mat frame;
    while (count--) {
        count++;
        cap >> frame;
        Mat img = frame.clone();

        cpuTimer.start();

        auto* rgb_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
        ImageUtil::bgr2rgb_packed(img.data, rgb_img, img.cols, img.rows);

        auto anchors = retinaFace.detect(rgb_img, img.cols, img.rows, 0.5);

        for (const auto& anchor : anchors) {
            auto* feature = (float*)malloc(arcFace.getFeatureSize() * sizeof(float));
            arcFace.recognize(rgb_img, img.cols, img.rows, anchor.kps, feature);

            retrievalCpuTimer.start();
            auto* I = new idx_t[10];
            auto* D = new float[10];

            index.search(1, feature, 10, D, I);

            retrievalCpuTimer.stop();

            float distance = D[0];

            cv::putText(img, "FPS = " + std::to_string(cpuTimer.getFPS()), Point(anchor.x, anchor.y - 50), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 255, 0));
            cv::putText(img, id_name_map[I[0]], Point(anchor.x, anchor.y - 30), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 255, 0));
            cv::putText(img, "similarity = " + std::to_string(distance), Point(anchor.x, anchor.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 255, 0));

            free(feature);
            delete[] I;
            delete[] D;
        }

        retrievalCpuTimer.print();
        cpuTimer.stop();

        ImageUtil::draw_faces(img, anchors);

        cv::imshow("image", img);
        cv::waitKey(1);
    }


    for (const auto& feature : features) {
        free(feature);
    }

}