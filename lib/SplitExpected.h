#pragma once

#include <expected>

#include "Range.h"
#include "Filter.h"


class SplitExpected : public Pipe {
    template<RangeSatisfiable Range>
    auto operator()(const Range& range) {
        auto expected = [](const Range::value_type& val) {return val.has_value();};
        auto unexpected = [](const Range::value_type& val) {return !val.has_value();};
        return std::make_pair(
            FilterRange(range, expected),
            FilterRange(range, unexpected)
        );
    }
};