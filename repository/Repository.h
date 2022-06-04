//
// Created by Abel Lee on 2022/6/3.
//

#ifndef CNIFACE_REPOSITORY_H
#define CNIFACE_REPOSITORY_H

#include <vector>
#include <string>
#include <unordered_map>

#include <faiss/IndexFlat.h>
#include <faiss/MetricType.h>
#include <faiss/MetaIndexes.h>
#include <faiss/impl/AuxIndexStructures.h>
#include <faiss/utils/distances.h>

#include <rocksdb/db.h>

#include "../common/common.h"

using namespace std;
using namespace faiss;

struct SearchResult {
    int64_t id;
    float distance;
};

class Repository {
private:
    explicit Repository(const string &name, int feature_dim);
    ~Repository();
    string name;
    rocksdb::DB* db{};
    IndexIDMap* index{};

    static const int featureSize = 512;
    static unordered_map<string, Repository*>* name_repo;
    static bool isInit;

    static Repository* getRepoByName(const string& repoName);


    vector<SearchResult> search(const string& featureBase64, int topk);
    void addWithId(int64_t featureId, const string& featureBase64);
    void deleteById(int64_t featureId);
    void destroy();

public:
    string getRepoName();
    int64_t size();

    static void initRepos();

    static int64_t size(const string& repoName);
    static vector<Repository*> listRepos();
    static void destroy(const string& repoName);
    static void addRepo(const string& repoName, int64_t featureDim);
    static vector<SearchResult> search(const string& repoName, const string& featureBase64, int topk);
    static void addWithId(const string& repoName, int64_t featureId, const string& featureBase64);
    static void deleteById(const string& repoName, int64_t featureId);
};


#endif //CNIFACE_REPOSITORY_H
