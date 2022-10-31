//
// Created by Abel Lee on 2022/10/2.
//

#ifndef CNIFACE_SEARCHRESULT_H
#define CNIFACE_SEARCHRESULT_H

#include <cstdint>

namespace cniface {

class SearchResult {
public:
    int64_t id;
    float score;
};

}



#endif //CNIFACE_SEARCHRESULT_H
