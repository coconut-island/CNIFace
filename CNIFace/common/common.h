//
// Created by Abel Lee on 2022/6/4.
//

#ifndef CNIFACE_COMMON_H
#define CNIFACE_COMMON_H

#include <stdexcept>
#include <string>

#define ThrowError(msg)         throw std::runtime_error(msg)
#define ThrowErrorIf(cond, msg) if (cond) throw std::runtime_error (msg)
#define CNIFACE_DIR std::string("./")
#define DEFAULT_FEATURE_DIM 512

#endif //CNIFACE_COMMON_H
