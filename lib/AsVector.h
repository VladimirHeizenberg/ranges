#pragma once

#include "Range.h"

#include <vector>

class AsVector : public Pipe {
public:
    template<class Range>
    auto operator()(Range&& range) {
        using ClearRange = std::remove_reference_t<Range>;
        std::vector<typename ClearRange::value_type> res;
        for (const auto& t : range) {
            res.push_back(t);
        }
        return res;
    }
};