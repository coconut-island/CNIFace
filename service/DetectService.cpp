//
// Created by Abel Lee on 2022/6/5.
//

#include "DetectService.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../utils/ImageUtil.h"
#include "../utils/base64.h"

DetectService::~DetectService() = default;

grpc::Status DetectService::detect(::grpc::ServerContext *context, const ::cniface::DetectRequest *request,
                                   ::cniface::DetectResponse *response) {
    response->set_code(0);
    response->set_message("OK");

    const auto& faceImageBase64 = request->faceimagebase64();
    auto imgStr = base64_decode(faceImageBase64);
    std::vector<char> base64_img(imgStr.begin(), imgStr.end());
    auto img = cv::imdecode(base64_img, cv::IMREAD_UNCHANGED);

    auto* rgb_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
    ImageUtil::bgr2rgb_packed(img.data, rgb_img, img.cols, img.rows);

    auto anchors = retinaFace.detect(rgb_img, img.cols, img.rows, request->score());

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

    free(rgb_img);
    return grpc::Status::OK;
}