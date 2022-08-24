//
// Created by Abel Lee on 2022/6/5.
//

#include "RecognitionService.h"

#include <opencv2/highgui.hpp>

#include "../utils/base64.h"
#include "../utils/MathUtil.h"
#include "../utils/ImageUtil.h"

#include "../common/common.h"



RecognitionService::RecognitionService(const string &model_dir) {
    m_w600k_r50 = new ArcFace(model_dir);
    m_w600k_mbf = new ArcFace(model_dir, "w600k_mbf");
}

RecognitionService::~RecognitionService() {
    delete m_w600k_r50;
    delete m_w600k_mbf;
}

grpc::Status RecognitionService::extractFeature(::grpc::ServerContext *context, const ::cniface::ExtractFeatureRequest *request,
                                   ::cniface::ExtractFeatureResponse *response) {
    response->set_code(0);
    response->set_message("OK");
    const auto& model = request->model();

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

    if (model == "w600k_mbf") {
        m_w600k_mbf->recognize(img.data, img.cols, img.rows, kps, feature);
    } else {
        m_w600k_r50->recognize(img.data, img.cols, img.rows, kps, feature);
    }

    MathUtil::normalize_L2(feature, DEFAULT_FEATURE_DIM);

    for (int i = 0; i < DEFAULT_FEATURE_DIM; ++i) {
        response->add_feature(feature[i]);
    }

    free(rgb_img);
    return grpc::Status::OK;
}

grpc::Status RecognitionService::similarity(::grpc::ServerContext *context, const ::cniface::SimilarityRequest *request,
                                            ::cniface::SimilarityResponse *response) {
    response->set_code(0);
    response->set_message("OK");

    const float* feature1 = request->feature1().data();
    const float* feature2 = request->feature2().data();

    auto distance = MathUtil::inner_product(feature1,
                                            feature2,
                                            DEFAULT_FEATURE_DIM);

    response->set_similarity(distance);
    return grpc::Status::OK;
}
