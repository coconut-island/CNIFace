//
// Created by Abel Lee on 2022/9/18.
//

#include "FileUtil.h"

#include <dirent.h>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>

#include <stdexcept>


int is_folder_exist(const char *path) {
    DIR *dp;
    if ((dp = opendir(path)) == NULL) {
        return 0;
    }
    closedir(dp);
    return -1;
}

bool file_exist(const std::string &path) {
    if (access(path.c_str(), F_OK) != 0) {
        return false;  // not exist
    }
    return true;
}

int make_dir(const char *path) {
    if (!is_folder_exist(path)) {
        return mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
    return 0;
}

int remove_dir(const char *path) {
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    if (d) {
        struct dirent *p;

        r = 0;

        while (!r && (p = readdir(d))) {
            int r2 = -1;
            char *buf;
            size_t len;

            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")) {
                continue;
            }

            len = path_len + strlen(p->d_name) + 2;
            buf = static_cast<char *>(malloc(len));

            if (buf) {
                struct stat statbuf;

                snprintf(buf, len, "%s/%s", path, p->d_name);

                if (!stat(buf, &statbuf)) {
                    if (S_ISDIR(statbuf.st_mode)) {
                        r2 = remove_dir(buf);
                    } else {
                        r2 = unlink(buf);
                    }
                }

                free(buf);
            }

            r = r2;
        }

        closedir(d);
    }

    if (!r) {
        r = rmdir(path);
    }

    return r;
}

int move_dir(const char *src, const char *dst, bool backup) {
    std::string option = "";
    if (backup) {
        option += "--backup=t";
    }
    std::string cmd = std::string("/bin/mv ") + option + " " + src + " " + dst;
    return system(cmd.c_str());
}

inline bool is_folder(const char *dir_name) {
    auto dir = opendir(dir_name);
    if (dir) {
        closedir(dir);
        return true;
    }
    return false;
}

inline bool is_folder(const std::string &dir_name) {
    return is_folder(dir_name.data());
}

file_filter_type default_ls_filter = [](const char *, const char *) {
    return true;
};

std::vector<std::string> ls(const std::string &dir_name, bool sub) {
    return for_each_file(dir_name, default_ls_filter, sub);
}

std::vector<std::string> ls_folder(const std::string &dir_name, bool sub) {
    return for_each_folder(dir_name, default_ls_filter, sub);
}

std::vector<std::string> for_each_file(const std::string &dir_name,
                                       file_filter_type filter, bool sub) {
    std::vector<std::string> v;
    auto dir = opendir(dir_name.data());
    struct dirent *ent;
    if (dir) {
        while ((ent = readdir(dir)) != NULL) {
            auto p =
                    std::string(dir_name).append({file_separator()}).append(ent->d_name);
            if (sub) {
                if (0 == strcmp(ent->d_name, "..") || 0 == strcmp(ent->d_name, ".")) {
                    continue;
                } else if (is_folder(p)) {
                    auto r = for_each_file(p, filter, sub);
                    v.insert(v.end(), r.begin(), r.end());
                    continue;
                }
            }
            if (sub || !is_folder(p))
                if (filter(dir_name.data(), ent->d_name)) v.emplace_back(p);
        }
        closedir(dir);
    }
    return v;
}

std::vector<std::string> for_each_folder(const std::string &dir_name,
                                         file_filter_type filter, bool sub) {
    std::vector<std::string> v;
    auto dir = opendir(dir_name.data());
    struct dirent *ent;
    if (dir) {
        while ((ent = readdir(dir)) != NULL) {
            auto p =
                    std::string(dir_name).append({file_separator()}).append(ent->d_name);
            if (sub) {
                if (0 == strcmp(ent->d_name, "..") || 0 == strcmp(ent->d_name, ".")) {
                    continue;
                } else if (is_folder(p)) {
                    auto r = for_each_folder(p, filter, sub);
                    v.insert(v.end(), r.begin(), r.end());
                    continue;
                }
            }
            if (sub || is_folder(p))
                if (filter(dir_name.data(), ent->d_name) &&
                    0 != strcmp(ent->d_name, "..") && 0 != strcmp(ent->d_name, "."))
                    v.emplace_back(ent->d_name);
        }
        closedir(dir);
    }
    return v;
}
