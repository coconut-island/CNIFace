//
// Created by Abel Lee on 2022/10/2.
//

#ifndef CNIFACE_REPOSITORYSERVICE_H
#define CNIFACE_REPOSITORYSERVICE_H

#include "../grpc/cniface.grpc.pb.h"
#include "../repository/Repository.h"
#include "../repository/SearchResult.h"


class RepositoryService final : public cniface::RepositoryService::Service {
public:
    explicit RepositoryService();
    explicit RepositoryService(const std::string& rootPath);
    ~RepositoryService() override;

    grpc::Status createRepository(::grpc::ServerContext* context, const ::cniface::CreateRepositoryRequest* request, ::cniface::CreateRepositoryResponse* response) override;
    grpc::Status deleteRepository(::grpc::ServerContext* context, const ::cniface::DeleteRepositoryRequest* request, ::cniface::DeleteRepositoryResponse* response) override;
    grpc::Status existRepository(::grpc::ServerContext* context, const ::cniface::ExistRepositoryRequest* request, ::cniface::ExistRepositoryResponse* response) override;
    grpc::Status listRepository(::grpc::ServerContext* context, const ::google::protobuf::Empty* request, ::cniface::ListRepositoryResponse* response) override;
    grpc::Status addOrUpdateItem(::grpc::ServerContext* context, const ::cniface::AddOrUpdateItemRequest* request, ::cniface::AddOrUpdateItemResponse* response) override;
    grpc::Status deleteItem(::grpc::ServerContext* context, const ::cniface::DeleteItemRequest* request, ::cniface::DeleteItemResponse* response) override;
    grpc::Status search(::grpc::ServerContext* context, const ::cniface::SearchRequest* request, ::cniface::SearchResponse* response) override;

private:
    Repository* mRepository;
};


#endif //CNIFACE_REPOSITORYSERVICE_H
