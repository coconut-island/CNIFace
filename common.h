//
// Created by Abel Lee on 2022/5/15.
//

#ifndef MAIN_COMMON_H
#define MAIN_COMMON_H

#include <vector>

#define PI 3.14159265

namespace cni {

class Point {
public:
    explicit Point();
    ~Point();
    float x{};
    float y{};
};

class Rect {
public:
    explicit Rect();
    ~Rect();
    float score{};
    float x{};
    float y{};
    float w{};
    float h{};
    float landmarks[5][2];
};

}

#endif //MAIN_COMMON_H
