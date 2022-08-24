//
// Created by Abel Lee on 2022/6/5.
//

#include "DetectService.h"
#include <opencv2/highgui.hpp>

#include "../utils/ImageUtil.h"
#include "../utils/base64.h"



DetectService::DetectService(const string &model_dir) {
    m_retinaFace10g = new RetinaFace(model_dir);
    m_retinaFace500m = new RetinaFace(model_dir, "det_500m");
    m_mNetCov2 = new MNetCov2(model_dir);
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

    for (const auto& anchor : anchors) {
        auto result = response->add_results();
        result->set_score(anchor.score);
        result->set_x(anchor.x);
        result->set_y(anchor.y);
        result->set_w(anchor.w);
        result->set_h(anchor.h);

        for (float kp : anchor.kps) {
            result->add_kps(kp);
        }
    }

    free(bgr_img);
    return grpc::Status::OK;
}
