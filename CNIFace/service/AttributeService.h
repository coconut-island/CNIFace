//
// Created by Abel Lee on 2022/8/25.
//

#ifndef CNIFACE_ATTRIBUTESERVICE_H
#define CNIFACE_ATTRIBUTESERVICE_H

#include "../grpc/cniface.grpc.pb.h"
#include "../genderage/GenderAge.h"


class AttributeService final : public cniface::AttributeService::Service {
public:
    explicit AttributeService(const std::string& model_dir, const std::vector<int> &cpu_devices);
    ~AttributeService() override;

    grpc::Status genderAge(::grpc::ServerContext *context, const ::cniface::GenderAgeRequest *request,
                        ::cniface::GenderAgeResponse *response) override;

private:
    GenderAge* m_genderAge;
};


#endif //CNIFACE_ATTRIBUTESERVICE_H
