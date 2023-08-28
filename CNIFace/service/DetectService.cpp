//
// Created by Abel Lee on 2022/6/5.
//

#include "DetectService.h"
#include <opencv2/highgui.hpp>

#include "../utils/ImageUtil.h"
#include "../utils/base64.h"



DetectService::DetectService(const string &model_dir, const std::vector<int> &cpu_devices) {
    m_retinaFace10g = new RetinaFace(model_dir, cpu_devices);
    m_retinaFace500m = new RetinaFace(model_dir, "det_500m", cpu_devices);
    m_mNetCov2 = new MNetCov2(model_dir, cpu_devices);
}

DetectService::~DetectService() {
    delete m_retinaFace10g;
    delete m_retinaFace500m;
    delete m_mNetCov2;
}

grpc::Status DetectService::detect(::grpc::ServerContext *context, const ::cniface::DetectRequest *request,
                                   ::cniface::DetectResponse *response) {
    response->set_code(0);
    response->set_message("OK");

    const auto& model = request->model();

    const auto& faceImageBase64 = request->faceimagebase64();
    auto imgStr = base64_decode(faceImageBase64);
    std::vector<char> base64_img(imgStr.begin(), imgStr.end());
    auto img = cv::imdecode(base64_img, cv::IMREAD_UNCHANGED);

    auto* bgr_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
    memcpy(bgr_img, img.data, img.rows * img.cols * 3 * sizeof(uint8_t));

    std::vector<Anchor> anchors{};

    if (model == "det_500m") {
        anchors = m_retinaFace500m->detect(bgr_img, img.cols, img.rows, request->score());
    } else if (model == "mnet_cov2") {
        anchors = m_mNetCov2->detect(bgr_img, img.cols, img.rows, request->score());
    } else {
        anchors = m_retinaFace10g->detect(bgr_img, img.cols, img.rows, request->score());
    }

    bool isReturnRecognitionSubImage = request->isreturnrecognitionsubimage();


    int subImageSize = 112 * 112 * 3;
    u_int8_t * subImage = nullptr;
    if (isReturnRecognitionSubImage) {
        subImage = (uint8_t*)malloc(subImageSize * sizeof(uint8_t));
    }

    for (const auto& anchor : anchors) {
        auto result = response->add_results();
        result->set_maskscore(anchor.mask_score);
        result->set_score(anchor.score);
        result->set_x(anchor.x);
        result->set_y(anchor.y);
        result->set_w(anchor.w);
        result->set_h(anchor.h);

        for (float kp : anchor.kps) {
            result->add_kps(kp);
        }


        if (isReturnRecognitionSubImage) {
            auto kps = anchor.kps;

            float dst[10] = {38.2946, 73.5318, 56.0252, 41.5493, 70.7299,
                                                    51.6963, 51.5014, 71.7366, 92.3655, 92.2041};
            float src[10];
            for (int i = 0; i < 5; i++) {
                src[i] = kps[2 * i];
                src[i + 5] = kps[2 * i + 1];
            }

            float M[6];
            ImageUtil::getAffineMatrix(src, dst, M);

            ImageUtil::warpAffineMatrix(bgr_img, subImage, img.cols, img.rows, 112, 112, M);

            std::vector<uchar> buffer;
            cv::Mat subImageMat(112, 112, CV_8UC3, subImage);
            cv::imencode(".png", subImageMat, buffer);

            string subImageBase64 = base64_encode(buffer.data(), buffer.size());

            result->set_subimagebase64(subImageBase64);
        }
    }

    if (isReturnRecognitionSubImage) {
        free(subImage);
    }

    free(bgr_img);
    return grpc::Status::OK;
}
