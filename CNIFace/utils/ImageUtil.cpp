//
// Created by Abel Lee on 2022/5/28.
//

#include "ImageUtil.h"

#include <emmintrin.h>
#include <algorithm>


void ImageUtil::swap_packed_rb(const uint8_t* src, uint8_t* dst, int img_width, int img_height) {
    for (int i = 0; i < img_width * img_height * 3; i += 3) {
        dst[i] = src[i + 2];
        dst[i + 1] = src[i + 1];
        dst[i + 2] = src[i];
    }
}

void ImageUtil::bgr2rgb_packed(const uint8_t* src, uint8_t* dst, int img_width, int img_height) {
    swap_packed_rb(src, dst, img_width, img_height);
}


void ImageUtil::rgb2bgr_packed(const uint8_t* src, uint8_t* dst, int img_width, int img_height) {
    swap_packed_rb(src, dst, img_width, img_height);
}


void ImageUtil::bilinear_resize(const uint8_t* dataSrc, uint8_t* dataDst, int src_width, int src_height, int dst_width, int dst_height) {
    double scale_x = (double)src_width / dst_width;
    double scale_y = (double)src_height / dst_height;

    int src_channels = 3;
    int dst_channels = 3;

    int stepSrc = src_width * src_channels;
    int stepDst = dst_width * dst_channels;

    for (int j = 0; j < dst_height; ++j) {
        auto fy = (float)((j + 0.5) * scale_y - 0.5);
        int sy = floor(fy);
        fy -= (float)sy;
        sy = std::min(sy, src_height - 2);
        sy = std::max(0, sy);

        short cbufy[2];
        cbufy[0] = static_cast<short>(((1.f - fy) * 2048));
        cbufy[1] = 2048 - cbufy[0];

        for (int i = 0; i < dst_width; ++i) {
            auto fx = (float)((i + 0.5) * scale_x - 0.5);
            int sx = floor(fx);
            fx -= (float)sx;

            if (sx < 0) {
                fx = 0, sx = 0;
            }
            if (sx >= src_width - 1) {
                fx = 0, sx = src_width - 2;
            }

            short cbufx[2];
            cbufx[0] = static_cast<short>(((1.f - fx) * 2048));
            cbufx[1] = 2048.0 - cbufx[0];

            for (int k = 0; k < src_channels; ++k) {
                *(dataDst + j * stepDst + 3 * i + k) = (*(dataSrc + sy * stepSrc + 3 * sx + k) * cbufx[0] * cbufy[0] +
                                                        *(dataSrc + (sy + 1) * stepSrc + 3 * sx + k) * cbufx[0] * cbufy[1] +
                                                        *(dataSrc + sy * stepSrc + 3 * (sx + 1) + k) * cbufx[1] * cbufy[0] +
                                                        *(dataSrc + (sy + 1) * stepSrc + 3 * (sx + 1) + k) * cbufx[1] * cbufy[1]) >> 22;
            }
        }
    }
}


void ImageUtil::getAffineMatrix(float* src_5pts, const float* dst_5pts, float* M) {
    float src[10], dst[10];
    memcpy(src, src_5pts, sizeof(float)*10);
    memcpy(dst, dst_5pts, sizeof(float)*10);

    float ptmp[2];
    ptmp[0] = ptmp[1] = 0;
    for (int i = 0; i < 5; ++i) {
        ptmp[0] += src[i];
        ptmp[1] += src[5+i];
    }
    ptmp[0] /= 5;
    ptmp[1] /= 5;
    for (int i = 0; i < 5; ++i) {
        src[i] -= ptmp[0];
        src[5+i] -= ptmp[1];
        dst[i] -= ptmp[0];
        dst[5+i] -= ptmp[1];
    }

    float dst_x = (dst[3]+dst[4]-dst[0]-dst[1])/2, dst_y = (dst[8]+dst[9]-dst[5]-dst[6])/2;
    float src_x = (src[3]+src[4]-src[0]-src[1])/2, src_y = (src[8]+src[9]-src[5]-src[6])/2;
    float theta = atan2(dst_x, dst_y) - atan2(src_x, src_y);

    float scale = sqrt(pow(dst_x, 2) + pow(dst_y, 2)) / sqrt(pow(src_x, 2) + pow(src_y, 2));
    float pts1[10];
    float pts0[2];
    float _a = sin(theta), _b = cos(theta);
    pts0[0] = pts0[1] = 0;
    for (int i = 0; i < 5; ++i) {
        pts1[i] = scale*(src[i]*_b + src[i+5]*_a);
        pts1[i+5] = scale*(-src[i]*_a + src[i+5]*_b);
        pts0[0] += (dst[i] - pts1[i]);
        pts0[1] += (dst[i+5] - pts1[i+5]);
    }
    pts0[0] /= 5;
    pts0[1] /= 5;

    float sqloss = 0;
    for (int i = 0; i < 5; ++i) {
        sqloss += ((pts0[0]+pts1[i]-dst[i])*(pts0[0]+pts1[i]-dst[i])
                   + (pts0[1]+pts1[i+5]-dst[i+5])*(pts0[1]+pts1[i+5]-dst[i+5]));
    }

    float square_sum = 0;
    for (float i : src) {
        square_sum += i * i;
    }
    for (int t = 0; t < 200; ++t) {
        _a = 0;
        _b = 0;
        for (int i = 0; i < 5; ++i) {
            _a += ((pts0[0]-dst[i])*src[i+5] - (pts0[1]-dst[i+5])*src[i]);
            _b += ((pts0[0]-dst[i])*src[i] + (pts0[1]-dst[i+5])*src[i+5]);
        }
        if (_b < 0) {
            _b = -_b;
            _a = -_a;
        }
        float _s = sqrt(_a*_a + _b*_b);
        _b /= _s;
        _a /= _s;

        for (int i = 0; i < 5; ++i) {
            pts1[i] = scale*(src[i]*_b + src[i+5]*_a);
            pts1[i+5] = scale*(-src[i]*_a + src[i+5]*_b);
        }

        float _scale = 0;
        for (int i = 0; i < 5; ++i) {
            _scale += ((dst[i]-pts0[0])*pts1[i] + (dst[i+5]-pts0[1])*pts1[i+5]);
        }
        _scale /= (square_sum*scale);
        for (float & i : pts1) {
            i *= (_scale / scale);
        }
        scale = _scale;

        pts0[0] = pts0[1] = 0;
        for (int i = 0; i < 5; ++i) {
            pts0[0] += (dst[i] - pts1[i]);
            pts0[1] += (dst[i+5] - pts1[i+5]);
        }
        pts0[0] /= 5;
        pts0[1] /= 5;

        float _sqloss = 0;
        for (int i = 0; i < 5; ++i) {
            _sqloss += ((pts0[0]+pts1[i]-dst[i])*(pts0[0]+pts1[i]-dst[i])
                        + (pts0[1]+pts1[i+5]-dst[i+5])*(pts0[1]+pts1[i+5]-dst[i+5]));
        }
        if (abs(_sqloss - sqloss) < 1e-2) {
            break;
        }
        sqloss = _sqloss;
    }

    for (int i = 0; i < 5; ++i) {
        pts1[i] += (pts0[0] + ptmp[0]);
        pts1[i+5] += (pts0[1] + ptmp[1]);
    }

    M[0] = _b*scale;
    M[1] = _a*scale;
    M[3] = -_a*scale;
    M[4] = _b*scale;
    M[2] = pts0[0] + ptmp[0] - scale*(ptmp[0]*_b + ptmp[1]*_a);
    M[5] = pts0[1] + ptmp[1] - scale*(-ptmp[0]*_a + ptmp[1]*_b);
}


void ImageUtil::warpAffineMatrix(const uint8_t* src_u, uint8_t* dst_u, int src_w, int src_h, int dst_w, int dst_h, const float *M) {
    float m[6];
    for (int i = 0; i < 6; i++) {
        m[i] = M[i];
    }

    float D = m[0] * m[4] - m[1] * m[3];
    D = D != 0 ? 1./D : 0;
    float A11 = m[4] * D, A22 = m[0] * D;
    m[0] = A11; m[1] *= -D;
    m[3] *= -D; m[4] = A22;
    float b1 = -m[0] * m[2] - m[1] * m[5];
    float b2 = -m[3] * m[2] - m[4] * m[5];
    m[2] = b1; m[5] = b2;

    for (int y= 0; y < dst_h; y++) {
        for (int x = 0; x < dst_w; x++) {
            float fx = m[0] * x + m[1] * y + m[2];
            float fy = m[3] * x + m[4] * y + m[5];

            int sy = (int)floor(fy);
            fy -= (float)sy;
            if (sy < 0 || sy >= src_h) continue;

            short cbufy[2];
            cbufy[0] = (short)((1.f - fy) * 2048);
            cbufy[1] = 2048 - cbufy[0];

            int sx = (int)floor(fx);
            fx -= (float)sx;
            if (sx < 0 || sx >= src_w) continue;

            short cbufx[2];
            cbufx[0] = (short)((1.f - fx) * 2048);
            cbufx[1] = 2048 - cbufx[0];

            if (sy == src_h - 1 || sx == src_w - 1)
                continue;
            for (int c = 0; c < 3; c++) {
                dst_u[3 * (y * dst_w + x) + c] = (
                                src_u[3 * (sy * src_w + sx) + c] * cbufx[0] * cbufy[0] +
                                src_u[3 * ((sy + 1) * src_w + sx) + c] * cbufx[0] * cbufy[1] +
                                src_u[3 * (sy * src_w + sx + 1) + c] * cbufx[1] * cbufy[0] +
                                src_u[3 * ((sy + 1) * src_w + sx + 1) + c] * cbufx[1] * cbufy[1]
                        ) >> 22;
            }
        }
    }
}

