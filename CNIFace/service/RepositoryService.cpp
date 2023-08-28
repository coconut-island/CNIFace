//
// Created by Abel Lee on 2022/10/2.
//

#include "RepositoryService.h"

RepositoryService::RepositoryService() {
    mRepository = new Repository();
}

RepositoryService::RepositoryService(const std::string &rootPath) {
    mRepository = new Repository(rootPath);
}

RepositoryService::~RepositoryService() {
    delete mRepository;
}

grpc::Status RepositoryService::createRepository(::grpc::ServerContext *context, const ::cniface::CreateRepositoryRequest *request,
                                    ::cniface::CreateRepositoryResponse *response) {
    response->set_code(0);
    response->set_message("OK");

    std::string repoName = request->reponame();
    int ret = mRepository->createRepository(repoName);
    if (ret != 0) {
        response->set_code(ret);
    }

    return grpc::Status::OK;
}

grpc::Status RepositoryService::deleteRepository(::grpc::ServerContext *context, const ::cniface::DeleteRepositoryRequest *request,
                                    ::cniface::DeleteRepositoryResponse *response) {
    response->set_code(0);
    response->set_message("OK");

    std::string repoName = request->reponame();
    int ret = mRepository->deleteRepository(repoName);
    if (ret != 0) {
        response->set_code(ret);
    }
    return grpc::Status::OK;
}

grpc::Status RepositoryService::existRepository(::grpc::ServerContext *context, const ::cniface::ExistRepositoryRequest *request,
                                   ::cniface::ExistRepositoryResponse *response) {
    response->set_code(0);
    response->set_message("OK");

    std::string repoName = request->reponame();
    int ret = mRepository->existRepository(repoName);
    if (ret != 0) {
        response->set_code(ret);
    }

    return grpc::Status::OK;
}

grpc::Status RepositoryService::listRepository(::grpc::ServerContext *context, const ::google::protobuf::Empty *request,
                                               ::cniface::ListRepositoryResponse *response) {
    response->set_code(0);
    response->set_message("OK");

    auto repoNames = mRepository->listRepository();

    for (auto& repoName : repoNames) {
        response->add_names(repoName);
    }

    return grpc::Status::OK;
}

grpc::Status RepositoryService::addOrUpdateItem(::grpc::ServerContext *context, const ::cniface::AddOrUpdateItemRequest *request,
                                   ::cniface::AddOrUpdateItemResponse *response) {


    response->set_code(0);
    response->set_message("OK");

    std::string repoName = request->reponame();
    auto id = request->id();
    const float* feature = request->feature().data();
    int ret = mRepository->addOrUpdateItem(repoName, id, feature);
    if (ret != 0) {
        response->set_code(ret);
    }

    return grpc::Status::OK;
}

grpc::Status RepositoryService::deleteItem(::grpc::ServerContext *context, const ::cniface::DeleteItemRequest *request,
                                           ::cniface::DeleteItemResponse *response) {
    response->set_code(0);
    response->set_message("OK");

    std::string repoName = request->reponame();
    auto id = request->id();
    int ret = mRepository->deleteItem(repoName, id);
    if (ret != 0) {
        response->set_code(ret);
    }

    return grpc::Status::OK;
}


grpc::Status RepositoryService::search(::grpc::ServerContext *context, const ::cniface::SearchRequest *request,
                                       ::cniface::SearchResponse *response) {

    response->set_code(0);
    response->set_message("OK");

    std::string repoName = request->reponame();
    auto topk = request->topk();
    const float* feature = request->feature().data();
    std::vector<cniface::SearchResult> searchResults = mRepository->search(repoName, topk, feature);

    for (const cniface::SearchResult& searchResult : searchResults) {
        auto result = response->add_results();
        result->set_id(searchResult.id);
        result->set_score(searchResult.score);
    }

    return grpc::Status::OK;
}
