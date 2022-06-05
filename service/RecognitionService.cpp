//
// Created by Abel Lee on 2022/6/5.
//

#include "RecognitionService.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../utils/base64.h"
#include "../utils/MathUtil.h"
#include "../utils/ImageUtil.h"

#include "../common/common.h"

RecognitionService::~RecognitionService() = default;

grpc::Status RecognitionService::extractFeature(::grpc::ServerContext *context, const ::cniface::ExtractFeatureRequest *request,
                                   ::cniface::ExtractFeatureResponse *response) {
    response->set_code(0);
    response->set_message("OK");
    const auto& faceImageBase64 = request->faceimagebase64();
    auto imgStr = base64_decode(faceImageBase64);
    std::vector<char> base64_img(imgStr.begin(), imgStr.end());
    auto img = cv::imdecode(base64_img, cv::IMREAD_UNCHANGED);

    auto* rgb_img = (uint8_t*)malloc(img.rows * img.cols * 3 * sizeof(uint8_t));
    ImageUtil::bgr2rgb_packed(img.data, rgb_img, img.cols, img.rows);

    vector<float> kps;
    for (const auto& kp : request->kps()) {
        kps.emplace_back(kp);
    }

    auto* feature = new float[DEFAULT_FEATURE_DIM];
    arcFace.recognize(img.data, img.cols, img.rows, kps, feature);
    MathUtil::normalize_L2(feature, DEFAULT_FEATURE_DIM);
    auto featureBase64 = base64_encode(feature, DEFAULT_FEATURE_DIM);

    response->set_featurebase64(featureBase64);
    free(rgb_img);
    return grpc::Status::OK;
}

grpc::Status RecognitionService::similarity(::grpc::ServerContext *context, const ::cniface::SimilarityRequest *request,
                                            ::cniface::SimilarityeResponse *response) {
    response->set_code(0);
    response->set_message("OK");

    const auto& featurebase64_1 = request->featurebase64_1();
    const auto& featurebase64_2 = request->featurebase64_2();

    auto distance = MathUtil::inner_product(
            (float*)base64_decode(featurebase64_1).c_str(),
            (float*)base64_decode(featurebase64_2).c_str(),
            DEFAULT_FEATURE_DIM);

    response->set_similarity(distance);
    return grpc::Status::OK;
}
