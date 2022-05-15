#include <iostream>
#include <iterator>
#include <cmath>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

#include "retinaface/anchor_generator.h"
#include "retinaface/config.h"
#include "retinaface/RetinaFaceDeploy.h"
#include "FacePreprocess.h"

#include "mobilefacenet/MobileFaceNetDeploy.h"


using namespace std;
using namespace tvm::runtime;
using namespace cv;


inline double count_angle(float landmark[5][2]) {
    double a = landmark[2][1] - (landmark[0][1] + landmark[1][1]) / 2;
    double b = landmark[2][0] - (landmark[0][0] + landmark[1][0]) / 2;
    double angle = atan(abs(b) / a) * 180 / PI;
    return angle;
}

void retinaOutput2Rects(const RetinaOutput& retinaOutput, vector<cni::Rect>& rects) {
    vector<Anchor> anchors = retinaOutput.result;
    float ratio_x = retinaOutput.ratio.x;
    float ratio_y = retinaOutput.ratio.y;

    for (auto& anchor : anchors) {
        cni::Rect rect;
        rect.score = anchor.score;
        rect.x = anchor.finalbox.x * ratio_x;
        rect.y = anchor.finalbox.y * ratio_y;
        rect.w = anchor.finalbox.width * ratio_x;
        rect.h = anchor.finalbox.height * ratio_y;

        for (int i = 0; i < anchor.pts.size(); ++i) {
            rect.landmarks[i][0] = anchor.pts[i].x * ratio_x;
            rect.landmarks[i][1] = anchor.pts[i].y * ratio_y;
        }
        rects.emplace_back(rect);
    }
}

Mat Zscore(const Mat &fc) {
    Mat mean, std;
    meanStdDev(fc, mean, std);
    Mat fc_norm = (fc - mean) / std;
    return fc_norm;
}

inline float getMold(const vector<float>& vec){
    int n = vec.size();
    float sum = 0.0;
    for (int i = 0; i < n; ++i)
        sum += vec[i] * vec[i];
    return sqrt(sum);
}

inline float CosineDistance(const vector<float>& lhs, const vector<float>& rhs){
    int n = lhs.size();
    assert(n == rhs.size());
    float tmp = 0.0;
    for (int i = 0; i<n; ++i)
        tmp += lhs[i] * rhs[i];
    return tmp / (getMold(lhs) * getMold(rhs));
}

void mat2Vector(const Mat& mat, vector<float>& feature) {
    if (mat.isContinuous()) {
        feature.assign((float*)mat.data, (float*)mat.data + mat.total() * mat.channels());
    } else {
        for (int i = 0; i < mat.rows; ++i) {
            feature.insert(feature.end(), mat.ptr<float>(i), mat.ptr<float>(i) + mat.cols * mat.channels());
        }
    }
}

int main(int argc, char *argv[]) {
    RetinaFaceDeploy retinaFaceDeploy("../models/relay/");
    MobileFaceNetDeploy mobileFaceNetDeploy("../models/relay/");

    auto img = imread("../images/1.jpg");
    auto retinaOutput = retinaFaceDeploy.forward(img);

    vector<cni::Rect> rects;
    retinaOutput2Rects(retinaOutput, rects);

    cni::Rect& rect = rects[0];

    float v1[5][2] = {
            {30.2946f + 8.0f, 51.6963f},
            {65.5318f + 8.0f, 51.5014f},
            {48.0252f + 8.0f, 71.7366f},
            {33.5493f + 8.0f, 92.3655f},
            {62.7299f + 8.0f, 92.2041f}
    };

    cv::Mat dst(5, 2, CV_32FC1, v1);
    memcpy(dst.data, v1, 2 * 5 * sizeof(float));

    double angle = count_angle(rect.landmarks);
    cv::Mat src(5, 2, CV_32FC1, angle);
    memcpy(src.data, rect.landmarks, 2 * 5 * sizeof(float));

    cv::Mat m = FacePreprocess::similarTransform(src, dst);
    cv::Mat aligned = img.clone();
    cv::warpPerspective(img, aligned, m, cv::Size(112, 112), INTER_LINEAR);
    resize(aligned, aligned, Size(112, 112), 0, 0, INTER_LINEAR);

    Mat fc = mobileFaceNetDeploy.forward(aligned);
    fc = Zscore(fc);

    std::vector<float> feature;
    mat2Vector(fc, feature);

    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "nothing" << endl;
        return -1;
    }

    int count = 0;
    Mat frame;
    while (count < 1000) {
        count++;
        cap >> frame;
        Mat result_cnn = frame.clone();

        RetinaOutput output_ = retinaFaceDeploy.forward(result_cnn);

        vector<cni::Rect> _rects;
        retinaOutput2Rects(output_, _rects);

        for (cni::Rect& _rect : _rects) {
            cv::Mat _dst(5, 2, CV_32FC1, v1);
            memcpy(_dst.data, v1, 2 * 5 * sizeof(float));

            double _angle = count_angle(_rect.landmarks);
            cv::Mat _src(5, 2, CV_32FC1, _angle);
            memcpy(_src.data, _rect.landmarks, 2 * 5 * sizeof(float));

            cv::Mat _m = FacePreprocess::similarTransform(_src, _dst);
            cv::Mat _aligned = frame.clone();
            cv::warpPerspective(result_cnn, _aligned, _m, cv::Size(112, 112), INTER_LINEAR);
            resize(_aligned, _aligned, Size(112, 112), 0, 0, INTER_LINEAR);

            Mat _fc = mobileFaceNetDeploy.forward(_aligned);
            _fc = Zscore(_fc);

            std::vector<float> _feature;
            mat2Vector(_fc, _feature);
            auto distance = CosineDistance(feature, _feature);

            auto _landmarks = _rect.landmarks;

            cv::putText(result_cnn, std::to_string(distance), Point(_rect.x, _rect.y), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255, 255, 0));
            cv::rectangle(result_cnn, Point(_rect.x, _rect.y), Point(_rect.w, _rect.h), cv::Scalar(0, 0, 255), 2);
            for (int i = 0; i < 5; ++i) {
                cv::circle(result_cnn, Point(_landmarks[i][0], _landmarks[i][1]), 3,
                           Scalar(0, 255, 0), FILLED, LINE_AA);
            }
        }

        cv::imshow("image", result_cnn);
        cv::waitKey(1);

    }
    return 0;
}