//
// Created by Abel Lee on 2022/5/28.
//

#include "Anchor.h"

Anchor::Anchor() = default;

Anchor::~Anchor() = default;

Anchor::Anchor(float score, float x, float y, float w, float h) {
    this->score = score;
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

