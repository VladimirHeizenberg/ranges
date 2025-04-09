#pragma once

#include <expected>

#include "Range.h"
#include "Filter.h"


class SplitExpected : public Pipe {
    template<class Range>
    auto operator()(const Range& range) {
        auto expected = [](const std::expected& val) {return val.has_value();};
        auto unexpected = [](const std::expected& val) {return !val.has_value();};
        return std::make_pair(
            FilterRange(range, expected),
            FilterRange(range, unexpected)
        );
    }
};