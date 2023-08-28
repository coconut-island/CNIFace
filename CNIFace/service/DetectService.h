//
// Created by Abel Lee on 2022/6/5.
//

#ifndef CNIFACE_DETECTSERVICE_H
#define CNIFACE_DETECTSERVICE_H

#include "../grpc/cniface.grpc.pb.h"
#include "../retinaface/RetinaFace.h"
#include "../mnet_cov2/MNetCov2.h"


class DetectService final : public cniface::DetectService::Service {
public:
    explicit DetectService(const std::string& model_dir, const std::vector<int> &cpu_devices);
    ~DetectService() override;

    grpc::Status detect(::grpc::ServerContext *context, const ::cniface::DetectRequest *request,
                        ::cniface::DetectResponse *response) override;

private:
    RetinaFace* m_retinaFace10g;
    RetinaFace* m_retinaFace500m;
    MNetCov2* m_mNetCov2;
};


#endif //CNIFACE_DETECTSERVICE_H
