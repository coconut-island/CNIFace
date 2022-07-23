//
// Created by Abel Lee on 2022/5/28.
//

#ifndef CNIFACE_IMAGEUTIL_H
#define CNIFACE_IMAGEUTIL_H

#include <cstdint>
#include <vector>
#include <cmath>
#include <cstring>

#include "../common/Anchor.h"


class ImageUtil {
public:
    static void bgr2rgb_packed(const uint8_t* src, uint8_t* dst, int img_width, int img_height);

    static void rgb2bgr_packed(const uint8_t* src, uint8_t* dst, int img_width, int img_height);

    static void swap_packed_rb(const uint8_t* src, uint8_t* dst, int img_width, int img_height);

    static void bilinear_resize(const uint8_t* dataSrc, uint8_t* dataDst, int src_width, int src_height, int dst_width, int dst_height);


    static void getAffineMatrix(float* src_5pts, const float* dst_5pts, float* M);

    static void warpAffineMatrix(const uint8_t* src_u, uint8_t* dst_u, int src_w, int src_h, int dst_w, int dst_h, const float *M);
};


#endif //CNIFACE_IMAGEUTIL_H
