//
// Created by Abel Lee on 2022/5/28.
//

#ifndef CNIFACE_ANCHOR_H
#define CNIFACE_ANCHOR_H

#include <iostream>
#include <vector>

using namespace std;

class Anchor {
public:
    explicit Anchor();
    explicit Anchor(float score, float x, float y, float w, float h);
    ~Anchor();

    float score{};
    float mask_score{};
    float x{};
    float y{};
    float w{};
    float h{};
    vector<float> kps = vector<float>(10);

    bool operator<(const Anchor &t) const {
        return score < t.score;
    }

    bool operator>(const Anchor &t) const {
        return score > t.score;
    }

    friend ostream &operator<<( ostream &output, const Anchor &t ) {
        output << "score: " << t.score
                << ", x: " << t.x
                << ", y: " << t.y
                << ", w: " << t.w
                << ", h: " << t.h;
        return output;
    }
};


#endif //CNIFACE_ANCHOR_H
