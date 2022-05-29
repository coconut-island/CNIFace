//
// Created by Abel Lee on 2022/5/29.
//

#include <cstddef>
#include <algorithm>

#include "MathUtil.h"


float MathUtil::inner_product(const float* a, const float* b, size_t size) {
    float dot = 0.0;
    for (size_t i = 0; i < size; ++i) {
        dot += a[i] * b[i] ;
    }
    return dot;
}

float MathUtil::cosine_similarity(const float* a, const float* b, size_t size) {
    float dot = 0.0, norm_a = 0.0, norm_b = 0.0 ;
    for (size_t i = 0; i < size; ++i) {
        dot += a[i] * b[i] ;
        norm_a += a[i] * a[i] ;
        norm_b += b[i] * b[i] ;
    }
    return dot / (sqrt(norm_a) * sqrt(norm_b)) ;
}
