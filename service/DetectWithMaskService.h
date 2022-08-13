//
// Created by Abel Lee on 2022/8/14.
//

#ifndef CNIFACE_DETECTWITHMASKSERVICE_H
#define CNIFACE_DETECTWITHMASKSERVICE_H

#include "../grpc/cniface.grpc.pb.h"
#include "../mnet_cov2/MNetCov2.h"

class DetectWithMaskService final : public cniface::DetectWithMaskService::Service {
public:
    explicit DetectWithMaskService(const std::string& model_dir);
    ~DetectWithMaskService() override;

    grpc::Status detect(::grpc::ServerContext *context, const ::cniface::DetectWithMaskRequest *request,
                        ::cniface::DetectWithMaskResponse *response) override;

private:
    MNetCov2* mNetCov2;
};


#endif //CNIFACE_DETECTWITHMASKSERVICE_H
