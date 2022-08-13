//
// Created by Abel Lee on 2022/8/14.
//

#include "DetectWithMaskService.h"
#include <opencv2/highgui.hpp>

#include "../utils/ImageUtil.h"
#include "../utils/base64.h"


DetectWithMaskService::DetectWithMaskService(const string &model_dir) {
    mNetCov2 = new MNetCov2(model_dir);
}

DetectWithMaskService::~DetectWithMaskService() {
    delete mNetCov2;
}

grpc::Status DetectWithMaskService::detect(::grpc::ServerContext *context, const ::cniface::DetectWithMaskRequest *request,
                                   ::cniface::DetectWithMaskResponse *response) {
    response->set_code(0);
    response->set_message("OK");

    const auto& faceImageBase64 = request->faceimagebase64();
    auto imgStr = base64_decode(faceImageBase64);
    std::vector<char> base64_img(imgStr.begin(), imgStr.end());
    auto img = cv::imdecode(base64_img, cv::IMREAD_UNCHANGED);

    auto* bgr_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
    memcpy(bgr_img, img.data, img.rows * img.cols * 3 * sizeof(uint8_t));

    auto anchors = mNetCov2->detect(bgr_img, img.cols, img.rows, request->score());

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
