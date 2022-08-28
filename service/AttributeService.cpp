//
// Created by Abel Lee on 2022/8/25.
//

#include "AttributeService.h"

#include <opencv2/highgui.hpp>

#include "../utils/ImageUtil.h"
#include "../utils/base64.h"

AttributeService::AttributeService(const string &model_dir) {
    m_genderAge = new GenderAge(model_dir);
}

AttributeService::~AttributeService() {
    delete m_genderAge;
}

grpc::Status AttributeService::genderAge(::grpc::ServerContext *context, const ::cniface::GenderAgeRequest *request,
                                         ::cniface::GenderAgeResponse *response) {

    response->set_code(0);
    response->set_message("OK");

    const auto& model = request->model();

    const auto& faceImageBase64 = request->faceimagebase64();
    auto imgStr = base64_decode(faceImageBase64);
    std::vector<char> base64_img(imgStr.begin(), imgStr.end());
    auto img = cv::imdecode(base64_img, cv::IMREAD_UNCHANGED);

    auto* bgr_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
    memcpy(bgr_img, img.data, img.rows * img.cols * 3 * sizeof(uint8_t));

    vector<float> kps;
    for (const auto& kp : request->kps()) {
        kps.emplace_back(kp);
    }

    auto result = m_genderAge->infer(bgr_img, img.cols, img.rows, kps);

    response->set_gender(result.gender);
    response->set_age(result.age);

    free(bgr_img);
    return grpc::Status::OK;
}
