//
// Created by Abel Lee on 2022/6/2.
//

#ifndef CNIFACE_CVUTIL_H
#define CNIFACE_CVUTIL_H

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "../common/Anchor.h"


class CVUtil {
public:

    static void draw_faces(cv::Mat& img, const vector<Anchor>& anchors);

};


#endif //CNIFACE_CVUTIL_H
