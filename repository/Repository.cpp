//
// Created by Abel Lee on 2022/6/3.
//

#include "Repository.h"

#include <dirent.h>

#include "../utils/base64.h"


bool Repository::isInit = false;
unordered_map<string, Repository*>* Repository::name_repo = new unordered_map<string, Repository*>();

Repository::Repository(const string &name, int feature_dim) {
    this->name = name;

    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status status = rocksdb::DB::Open(options, CNIFACE_DIR + name + ".db", &this->db);
    ThrowErrorIf(!status.ok(), "Create and open rocksdb Failed! status:" + to_string(status.code()));

    this->db->Put(rocksdb::WriteOptions(), "meta:db_name", name);
    this->db->Put(rocksdb::WriteOptions(), "meta:feature_dim", to_string(feature_dim));

    auto* _index = new IndexFlat(feature_dim, faiss::METRIC_INNER_PRODUCT);
    this->index = new IndexIDMap(_index);

    rocksdb::Iterator *it = this->db->NewIterator(rocksdb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        if (it->key().starts_with("meta:")) {
            continue;
        }
        faiss::Index::idx_t idx = stol(it->key().ToString());
        this->index->add_with_ids(1, (float*)base64_decode(it->value().ToString()).c_str(), &idx);
    }

}

Repository::~Repository() {
    name_repo->erase(name);
    delete index;
    db->Close();
    delete db;
};

void Repository::initRepos() {
    isInit = false;
    cout << "Start Init Local Repos!" << endl;
    name_repo->clear();

    vector<string> db_names;
    DIR *pDir;
    struct dirent* ptr;
    if(!(pDir = opendir(CNIFACE_DIR.c_str()))){
        cout << "Folder doesn't Exist!" << endl;
        return;
    }
    while((ptr = readdir(pDir)) != nullptr) {
        string string_dir_name = string(ptr->d_name);
        if (
                string_dir_name.size() > 3 &&
                string_dir_name.substr(string_dir_name.size() - 3, string_dir_name.size()) == ".db"
                ){
            string db_dir_name(ptr->d_name);
            db_names.emplace_back(db_dir_name.substr(0, db_dir_name.size() - 3));
        }
    }
    closedir(pDir);

    cout << "Local Repo Count = " << db_names.size() << endl;
    for (const auto& db_name : db_names) {
        cout << "Loading Local Repo: " << db_name << endl;
        auto* repository = new Repository(db_name, featureSize);
        (*name_repo)[db_name] = repository;
    }
    isInit = true;
}

Repository *Repository::getRepoByName(const string &repoName) {
    ThrowErrorIf(!isInit, "Repository Module Not Finished Init!");
    if (name_repo->find(repoName) == name_repo->end()) {
        ThrowError("Repo Not Exist! name = " + repoName);
    }
    return (*name_repo)[repoName];
}

vector<Repository*> Repository::listRepos() {
    ThrowErrorIf(!isInit, "Repository Module Not Finished Init!");
    vector<Repository*> repos;
    for (auto& item : (*name_repo)) {
        repos.emplace_back(item.second);
    }
    return repos;
}

void Repository::destroy() {
    delete this;
    rocksdb::DestroyDB(CNIFACE_DIR + name + ".db", rocksdb::Options());
}

void Repository::destroy(const string &repoName) {
    ThrowErrorIf(!isInit, "Repository Module Not Finished Init!");
    auto* repo = getRepoByName(repoName);
    repo->destroy();
}

void Repository::addRepo(const string &repoName, int64_t featureDim) {
    ThrowErrorIf(!isInit, "Repository Module Not Finished Init!");
    if (name_repo->find(repoName) != name_repo->end()) {
        cout << "Repository Already Exists! repo name: " << repoName << endl;
        return;
    }
    auto* repository = new Repository(repoName, featureDim);
    (*name_repo)[repoName] = repository;
}

vector<SearchResult> Repository::search(const string &featureBase64, int topk) {
    vector<SearchResult> searchResults;
    auto* I = new faiss::Index::idx_t[topk];
    auto* D = new float[topk];

    this->index->search(1, (float*)base64_decode(featureBase64).c_str(), topk, D, I);

    for (int i = 0; i < topk; ++i) {
        if (I[i] <= 0) {
            continue;
        }
        SearchResult searchResult{};
        searchResult.id = I[i];
        searchResult.distance = D[i];
        searchResults.emplace_back(searchResult);
    }

    return searchResults;
}

vector<SearchResult> Repository::search(const string &repoName, const string &featureBase64, int topk) {
    ThrowErrorIf(!isInit, "Repository Module Not Finished Init!");
    auto* repo = getRepoByName(repoName);
    return repo->search(featureBase64, topk);
}

void Repository::addWithId(int64_t featureId, const string &featureBase64) {
    string s_featureId = to_string(featureId);
    bool valueFount = false;
    string value;
    if (db->KeyMayExist(rocksdb::ReadOptions(), s_featureId, &value, &valueFount)) {
        cout << "Key Already Exist! key = " << s_featureId << endl;
        return;
    }
    auto status = db->Put(rocksdb::WriteOptions(), s_featureId, featureBase64);
    ThrowErrorIf(!status.ok(), "Rocksdb Put Failed! msg = " + status.ToString());
    index->add_with_ids(1, (float*)(base64_decode(featureBase64).c_str()), &featureId);
}

void Repository::addWithId(const string &repoName, int64_t featureId, const string &featureBase64) {
    ThrowErrorIf(!isInit, "Repository Module Not Finished Init!");
    auto* repo = getRepoByName(repoName);
    repo->addWithId(featureId, featureBase64);
}

void Repository::deleteById(int64_t featureId) {
    faiss::IDSelectorBatch del_id(1, &featureId);
    index->remove_ids(del_id);
    auto status = db->SingleDelete(rocksdb::WriteOptions(), to_string(featureId));
    ThrowErrorIf(!status.ok(), "Rocksdb SingleDelete Failed! msg = " + status.ToString());
}

void Repository::deleteById(const string &repoName, int64_t featureId) {
    ThrowErrorIf(!isInit, "Repository Module Not Finished Init!");
    auto* repo = getRepoByName(repoName);
    repo->deleteById(featureId);
}

int64_t Repository::size() {
    return this->index->ntotal;
}

int64_t Repository::size(const string& repoName) {
    ThrowErrorIf(!isInit, "Repository Module Not Finished Init!");
    auto* repo = getRepoByName(repoName);
    return repo->size();
}

string Repository::getRepoName() {
    return this->name;
}

