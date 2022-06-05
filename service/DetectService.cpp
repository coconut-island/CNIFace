//
// Created by Abel Lee on 2022/6/5.
//

#include "DetectService.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../utils/base64.h"

DetectService::~DetectService() = default;

grpc::Status DetectService::detect(::grpc::ServerContext *context, const ::cniface::DetectRequest *request,
                                   ::cniface::DetectResponse *response) {
    response->set_code(0);
    response->set_message("OK");

    auto faceImageBase64 = request->faceimagebase64();
    auto imgStr = base64_decode(faceImageBase64);
    std::vector<char> base64_img(imgStr.begin(), imgStr.end());
    auto img = cv::imdecode(base64_img, cv::COLOR_BGR2RGB);

    auto anchors = retinaFace.detect(img.data, img.cols, img.rows, request->score());

    for (const auto& anchor : anchors) {
        auto result = response->add_results();
        result->set_score(anchor.score);
        result->set_x(anchor.x);
        result->set_y(anchor.y);
        result->set_w(anchor.w);
        result->set_h(anchor.h);
        for (int i = 0; i < anchor.kps.size(); ++i) {
            result->set_kps(i, anchor.kps[i]);
        }
    }
    return Service::detect(context, request, response);
}