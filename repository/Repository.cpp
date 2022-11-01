//
// Created by Abel Lee on 2022/9/18.
//

#include "Repository.h"

Repository::Repository() {
    init();
}

Repository::Repository(const std::string& rootPath) {
    mRootPath = rootPath;
    init();
}

Repository::~Repository() {
    for (auto &engineEntry : mEngineMap) {
//        Dump(engineEntry.second);
        Close(engineEntry.second);
    }
}

void Repository::init() {
    LOG(INFO) << "Starting init repositories.";
    auto repoNames = ls_folder(mRootPath);

    for (auto& repoName : repoNames) {
        LOG(INFO) << "Starting init repository, name = " << repoName;
        void *engine;
        tig_gamma::Config config;
        std::string repoPath = mRootPath + "/" + repoName;
        config.SetPath(repoPath);
        std::string repoLogPath = repoPath + "/log";
        std::string logDir = repoLogPath;
        config.SetLogDir(logDir);
        char *config_str = nullptr;
        int engineConfigLen = 0;
        config.Serialize(&config_str, &engineConfigLen);
        engine = Init(config_str, engineConfigLen);

        int ret = Load(engine);
        if (ret != 0) {
            LOG(ERROR) << "Load Engine Error! ret = " << ret;
            return;
        }

        ret = BuildIndex(engine);
        if (ret != 0) {
            LOG(ERROR) << "Build Index Failed! ret = " << ret;
            return;
        }
        int n_index_status;
        do {
            char *status = nullptr;
            int engineStatusLen = 0;
            GetEngineStatus(engine, &status, &engineStatusLen);
            tig_gamma::EngineStatus engine_status;
            engine_status.Deserialize(status, engineStatusLen);
            free(status);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            n_index_status = engine_status.IndexStatus();
        } while (n_index_status != 2);

        mEngineMap[repoName] = engine;
        LOG(INFO) << "Loaded Repo Success! repo name = " << repoName;
    }

    mIsLoad = true;
    LOG(INFO) << "Finished init repositories.";
}

bool Repository::isLoad() {
    return mIsLoad;
}

int Repository::createRepository(std::string &repoName) {
    if (existRepository(repoName)) {
        LOG(INFO) << "Repo Already Exist!";
        return -1;
    }

    void *engine;
    tig_gamma::Config config;
    std::string repoPath = mRootPath + "/" + repoName;
    config.SetPath(repoPath);
    std::string repoLogPath = repoPath + "/log";
    std::string logDir = repoLogPath;
    config.SetLogDir(logDir);
    char *config_str = nullptr;
    int engineConfigLen = 0;
    config.Serialize(&config_str, &engineConfigLen);
    engine = Init(config_str, engineConfigLen);

    std::string table_name = "default_table";
    std::string retrieval_type = "FLAT";
    std::string store_type = "MemoryOnly";
    std::string kFLATParam = R"({"metric_type" : "InnerProduct"})";
    int indexing_size = 300000;
    std::string model_id = "model";

    tig_gamma::TableInfo table;
    table.SetName(table_name);
    table.SetRetrievalType(retrieval_type);
    table.SetRetrievalParam(kFLATParam);
    table.SetIndexingSize(indexing_size);

    struct tig_gamma::FieldInfo field_info;
    field_info.name = mIdFieldName;
    field_info.is_index = true;
    field_info.data_type = tig_gamma::DataType::STRING;
    table.AddField(field_info);

    struct tig_gamma::VectorInfo vector_info;
    vector_info.name = mFeatureVectorName;
    vector_info.data_type = tig_gamma::DataType::FLOAT;
    vector_info.is_index = true;
    vector_info.dimension = mDim;
    vector_info.model_id = model_id;
    vector_info.store_type = store_type;
    vector_info.store_param = "{\"cache_size\": 16}";
    vector_info.has_source = false;

    table.AddVectorInfo(vector_info);

    char *table_str = nullptr;
    int table_len = 0;
    table.Serialize(&table_str, &table_len);

    int ret = CreateTable(engine, table_str, table_len);
    free(table_str);
    if (ret != 0) {
        LOG(ERROR) << "Create Table Failed! ret = " << ret;
        return - 1;
    }

    ret = BuildIndex(engine);
    if (ret != 0) {
        LOG(ERROR) << "Build Index Failed! ret = " << ret;
        return - 2;
    }
    int n_index_status;
    do {
        char *status = nullptr;
        int engineStatusLen = 0;
        GetEngineStatus(engine, &status, &engineStatusLen);
        tig_gamma::EngineStatus engine_status;
        engine_status.Deserialize(status, engineStatusLen);
        free(status);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        n_index_status = engine_status.IndexStatus();
    } while (n_index_status != 2);

    mEngineMap[repoName] = engine;
    return 0;
}

int Repository::deleteRepository(std::string &repoName) {
    std::string repoPath = mRootPath + "/" + repoName;
    int res = remove_dir(repoPath.c_str());
    if (res != 0) {
        LOG(ERROR) << "Remove Dir Failed! dir path = " << repoPath
                   << ", res = " << res;
        return res;
    }
    mEngineMap.erase(repoName);
    return 0;
}

bool Repository::existRepository(std::string &repoName) {
    std::string repoPath = mRootPath + "/" + repoName;
    return is_folder_exist(repoPath.c_str());
}

std::vector<std::string> Repository::listRepository() {
    std::vector<std::string> repoNames;
    for (auto &engineEntry : mEngineMap) {
        repoNames.emplace_back(engineEntry.first);
    }
    return repoNames;
}

int Repository::addOrUpdateItem(std::string &repoName, int64_t id, const float *feature) {
    return addOrUpdateItem(repoName, id, feature, mDim);
}

int Repository::addOrUpdateItem(std::string &repoName, int64_t id, const float *feature, int featureSize) {
    if (mEngineMap.find(repoName) == mEngineMap.end()) {
        LOG(ERROR) << "Repo Not Exist! repo name = " << repoName;
        return -1;
    }

    void* engine = mEngineMap[repoName];

    tig_gamma::Doc doc;
    doc.SetKey(std::to_string(id));

    tig_gamma::Field fieldId;
    fieldId.name = mIdFieldName;
    fieldId.datatype = tig_gamma::DataType::STRING;
    std::string data = std::to_string(id);
    fieldId.value = data;
    doc.AddField(std::move(fieldId));

    tig_gamma::Field fieldVector;
    fieldVector.name = mFeatureVectorName;
    fieldVector.datatype = tig_gamma::DataType::VECTOR;
    fieldVector.value = std::string((char *)(feature), featureSize * sizeof(float));
    doc.AddField(std::move(fieldVector));

    char *doc_str = nullptr;
    int doc_len = 0;
    doc.Serialize(&doc_str, &doc_len);
    int res = AddOrUpdateDoc(engine, doc_str, doc_len);
    free(doc_str);
    if (res != 0) {
        LOG(ERROR) << "AddOrUpdateDoc Failed! res= " << res;
        return -1;
    }
    return 0;
}

int Repository::deleteItem(std::string &repoName, int64_t id) {
    if (mEngineMap.find(repoName) == mEngineMap.end()) {
        LOG(ERROR) << "Repo Not Exist! repo name = " << repoName;
        return -1;
    }

    std::string docId = std::to_string(id);
    void* engine = mEngineMap[repoName];

    int res = DeleteDoc(engine, docId.c_str(), docId.size());
    if (res != 0) {
        LOG(ERROR) << "DeleteDoc Failed! res= " << res << ", doc id = " << docId;
        return -1;
    }

    return 0;
}

std::vector<cniface::SearchResult> Repository::search(std::string &repoName, int topk, const float *feature) {
    return search(repoName, topk, feature, mDim);
}

std::vector<cniface::SearchResult> Repository::search(std::string &repoName, int topk, const float *feature, int featureSize) {
    std::vector<cniface::SearchResult> searchResults;
    if (mEngineMap.find(repoName) == mEngineMap.end()) {
        LOG(ERROR) << "Repo Not Exist! repo name = " << repoName;
        return searchResults;
    }

    void* engine = mEngineMap[repoName];

    int req_num = 1;
    struct tig_gamma::VectorQuery vector_query;
    vector_query.name = mFeatureVectorName;
    vector_query.value = std::string((char*)feature, featureSize * sizeof(float));
    vector_query.min_score = -1;
    vector_query.max_score = 2;
    vector_query.boost = 0.1;
    vector_query.has_boost = 0;

    tig_gamma::Request request;
    request.SetTopN(topk);
    request.AddVectorQuery(vector_query);
    request.SetReqNum(req_num);
    request.SetBruteForceSearch(0);
    request.SetHasRank(true);
    std::string retrieval_params = R"({"metric_type":"InnerProduct"})";
    request.SetRetrievalParams(retrieval_params);
    request.SetMultiVectorRank(0);
    request.SetL2Sqrt(false);
    request.AddField(mIdFieldName);

    char *request_str, *response_str;
    int request_len, response_len;

    request.Serialize(&request_str, &request_len);
    int ret = Search(engine, request_str, request_len, &response_str,
                     &response_len);
    free(request_str);

    if (ret != 0) {
        LOG(ERROR) << "Search Filed! ret = " << ret;
        return searchResults;
    }

    tig_gamma::Response response;
    response.Deserialize(response_str, response_len);
    free(response_str);

    std::vector<struct tig_gamma::SearchResult> &results = response.Results();

    for (auto &result : results) {
        std::vector<struct tig_gamma::ResultItem> &result_items = result.result_items;
        for (auto &result_item : result_items) {
            cniface::SearchResult searchResult{};
            for (int i = 0; i < result_item.names.size(); ++i) {
                if (result_item.names[i] == mIdFieldName) {
                    searchResult.id = stoll(result_item.values[i]);
                }
            }
            searchResult.score = (float)result_item.score;
            searchResults.emplace_back(searchResult);
        }
    }

    return searchResults;
}
