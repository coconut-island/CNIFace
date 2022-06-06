//
// Created by Abel Lee on 2022/6/5.
//

#include "RepositoryService.h"

#include "../repository/Repository.h"


RepositoryService::RepositoryService() {
    Repository::initRepos();
}

RepositoryService::~RepositoryService() = default;

grpc::Status RepositoryService::listRepo(::grpc::ServerContext *context, const ::google::protobuf::Empty *request,
                                         ::cniface::ListRepositoryResponse *response) {
    response->set_code(0);
    response->set_message("OK");
    auto repos = Repository::listRepos();

    for (const auto& repo : repos ) {
        auto repository = response->add_repositories();
        repository->set_name(repo->getRepoName());
        repository->set_size(repo->size());
    }

    return grpc::Status::OK;
}

grpc::Status RepositoryService::addRepo(::grpc::ServerContext *context, const ::cniface::AddRepoRequest *request,
                                        ::cniface::AddRepoResponse *response) {
    response->set_code(0);
    response->set_message("OK");
    Repository::addRepo(request->reponame(), DEFAULT_FEATURE_DIM);
    return grpc::Status::OK;
}

grpc::Status RepositoryService::deleteRepo(::grpc::ServerContext *context, const ::cniface::DeleteRepoRequest *request,
                                           ::cniface::DeleteRepoResponse *response) {
    response->set_code(0);
    response->set_message("OK");
    Repository::destroy(request->reponame());
    return grpc::Status::OK;
}

grpc::Status RepositoryService::search(::grpc::ServerContext *context, const ::cniface::SearchRequest *request,
                                       ::cniface::SearchResponse *response) {
    response->set_code(0);
    response->set_message("OK");
    auto searchResults = Repository::search(request->reponame(), request->featurebase64(), request->topk());
    for (const auto& searchResult : searchResults) {
        auto result = response->add_results();
        result->set_featureid(result->featureid());
        result->set_distance(result->distance());
    }

    return grpc::Status::OK;
}

grpc::Status RepositoryService::addFeature(::grpc::ServerContext *context, const ::cniface::AddFeatureRequest *request,
                                           ::cniface::AddFeatureResponse *response) {
    response->set_code(0);
    response->set_message("OK");
    Repository::addWithId(request->reponame(), request->featureid(), request->featurebase64());
    return grpc::Status::OK;
}

grpc::Status RepositoryService::deleteFeature(::grpc::ServerContext *context, const ::cniface::DeleteFeatureRequest *request,
                                              ::cniface::DeleteFeatureResponse *response) {
    response->set_code(0);
    response->set_message("OK");
    Repository::deleteById(request->reponame(), request->featureid());
    return grpc::Status::OK;
}
