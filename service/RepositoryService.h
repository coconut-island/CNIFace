//
// Created by Abel Lee on 2022/6/5.
//

#ifndef CNIFACE_REPOSITORYSERVICE_H
#define CNIFACE_REPOSITORYSERVICE_H

#include "../grpc/cniface.grpc.pb.h"


class RepositoryService final : public cniface::RepositoryService::Service {
public:
    explicit RepositoryService();
    ~RepositoryService() override;

    grpc::Status listRepo(::grpc::ServerContext *context, const ::google::protobuf::Empty *request,
                          ::cniface::ListRepositoryResponse *response) override;

    grpc::Status addRepo(::grpc::ServerContext *context, const ::cniface::AddRepoRequest *request,
                         ::cniface::AddRepoResponse *response) override;

    grpc::Status deleteRepo(::grpc::ServerContext *context, const ::cniface::DeleteRepoRequest *request,
                            ::cniface::DeleteRepoResponse *response) override;

    grpc::Status search(::grpc::ServerContext *context, const ::cniface::SearchRequest *request,
                        ::cniface::SearchResponse *response) override;

    grpc::Status addFeature(::grpc::ServerContext *context, const ::cniface::AddFeatureRequest *request,
                            ::cniface::AddFeatureResponse *response) override;

    grpc::Status deleteFeature(::grpc::ServerContext *context, const ::cniface::DeleteFeatureRequest *request,
                               ::cniface::DeleteFeatureResponse *response) override;
};


#endif //CNIFACE_REPOSITORYSERVICE_H
