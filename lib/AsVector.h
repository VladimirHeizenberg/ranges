#pragma once
#include "Range.h"
#include <vector>

class AsVector : public Pipe {
public:
    template<RangeSatisfiable Range>
    std::vector<typename Range::value_type> operator()(const Range& range) const {
        std::vector<typename Range::value_type> res;
        for (auto t : range) {
            res.push_back(t);
        }
        return res;
    }
};