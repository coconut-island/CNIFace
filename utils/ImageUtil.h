//
// Created by Abel Lee on 2022/5/28.
//

#ifndef CNIFACE_IMAGEUTIL_H
#define CNIFACE_IMAGEUTIL_H

#include <cstdint>
#include <vector>

#include <opencv2/highgui.hpp>

#include "../common/Anchor.h"


class ImageUtil {
public:
    static void bilinear_resize(const uint8_t* dataSrc, uint8_t* dataDst, int src_width, int src_height, int dst_width, int dst_height);

    static cv::Mat draw_faces(cv::Mat& img, const vector<Anchor>& anchors);

    static void getAffineMatrix(float* src_5pts, const float* dst_5pts, float* M);

    static void warpAffineMatrix(const uint8_t* src_u, uint8_t* dst_u, int src_w, int src_h, int dst_w, int dst_h, const float *M);
};


#endif //CNIFACE_IMAGEUTIL_H
