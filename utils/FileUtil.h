//
// Created by Abel Lee on 2022/9/18.
//

#ifndef CNIFACE_FILEUTIL_H
#define CNIFACE_FILEUTIL_H

#include <string>
#include <vector>


int is_folder_exist(const char *path);

bool file_exist(const std::string &path);

int make_dir(const char *path);

int remove_dir(const char *dir);

int move_dir(const char *src, const char *dst, bool backup = false);

inline char file_separator() { return '/'; }

using file_filter_type = std::function<bool(const char *, const char *)>;

std::vector<std::string> ls(const std::string &dir_name, bool sub = false);

std::vector<std::string> ls_folder(const std::string &dir_name,
                                   bool sub = false);

std::vector<std::string> for_each_file(const std::string &dir_name,
                                       file_filter_type filter,
                                       bool sub = false);

std::vector<std::string> for_each_folder(const std::string &dir_name,
                                         file_filter_type filter,
                                         bool sub = false);



#endif //CNIFACE_FILEUTIL_H
