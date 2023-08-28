//
// Created by Abel Lee on 2022/9/18.
//

#ifndef CNIFACE_REPOSITORY_H
#define CNIFACE_REPOSITORY_H

#include <cstring>
#include <thread>
#include <iostream>
#include <unordered_map>
#include <gamma/c_api/gamma_api.h>
#include <gamma/c_api/api_data/gamma_config.h>
#include <gamma/c_api/api_data/gamma_table.h>
#include <gamma/c_api/api_data/gamma_doc.h>
#include <gamma/c_api/api_data/gamma_request.h>
#include <gamma/c_api/api_data/gamma_response.h>
#include <gamma/c_api/api_data/gamma_engine_status.h>

#include "../utils/FileUtil.h"
#include "../utils/Log.h"
#include "./SearchResult.h"


class Repository {

public:
    explicit Repository();
    explicit Repository(const std::string& rootPath);
    ~Repository();
    bool isLoad();
    int createRepository(std::string &repoName);
    int deleteRepository(std::string &repoName);
    bool existRepository(std::string &repoName);
    std::vector<std::string> listRepository();
    int addOrUpdateItem(std::string &repoName, int64_t id, const float* feature);
    int addOrUpdateItem(std::string &repoName, int64_t id, const float* feature, int featureSize);
    int deleteItem(std::string &repoName, int64_t id);
    std::vector<cniface::SearchResult> search(std::string &repoName, int topk, const float* feature);
    std::vector<cniface::SearchResult> search(std::string &repoName, int topk, const float* feature, int featureSize);

private:
    void init();

private:
    bool mIsLoad = false;
    std::unordered_map<std::string, void*> mEngineMap;
    std::string mRootPath = "./repo_root";
    int mDim = 512;
    std::string mFeatureVectorName = "feature";
    std::string mIdFieldName = "_id";
};


#endif //CNIFACE_REPOSITORY_H
