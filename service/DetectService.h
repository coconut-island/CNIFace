//
// Created by Abel Lee on 2022/6/5.
//

#ifndef CNIFACE_DETECTSERVICE_H
#define CNIFACE_DETECTSERVICE_H

#include "../grpc/cniface.grpc.pb.h"
#include "../retinaface/RetinaFace.h"


class DetectService final : public cniface::DetectService::Service {
public:
    ~DetectService() override;

    grpc::Status detect(::grpc::ServerContext *context, const ::cniface::DetectRequest *request,
                        ::cniface::DetectResponse *response) override;

private:
    RetinaFace retinaFace{"../models/relay/"};
};


#endif //CNIFACE_DETECTSERVICE_H
