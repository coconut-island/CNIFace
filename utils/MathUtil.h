//
// Created by Abel Lee on 2022/5/29.
//

#ifndef CNIFACE_MATHUTIL_H
#define CNIFACE_MATHUTIL_H


class MathUtil {

public:
    static void normalize_L2(float* data, size_t size);

    static float inner_product(const float* a, const float* b, size_t size);

    static float cosine_similarity(const float* a, const float* b, size_t size);

};


#endif //CNIFACE_MATHUTIL_H
