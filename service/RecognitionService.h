//
// Created by Abel Lee on 2022/6/5.
//

#ifndef CNIFACE_RECOGNITIONSERVICE_H
#define CNIFACE_RECOGNITIONSERVICE_H

#include "../grpc/cniface.grpc.pb.h"
#include "../arcface/ArcFace.h"


class RecognitionService final : public cniface::RecognitionService::Service {
public:
    ~RecognitionService() override;

    grpc::Status extractFeature(::grpc::ServerContext *context, const ::cniface::ExtractFeatureRequest *request,
                                ::cniface::ExtractFeatureResponse *response) override;

    grpc::Status similarity(::grpc::ServerContext *context, const ::cniface::SimilarityRequest *request,
                            ::cniface::SimilarityResponse *response) override;

private:
    ArcFace arcFace{"../models/relay/"};
};


#endif //CNIFACE_RECOGNITIONSERVICE_H
