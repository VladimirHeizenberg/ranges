#pragma once

#include <expected>

#include "Range.h"
#include "Filter.h"


class SplitExpected : public Pipe {
public:
    template<class Range>
    auto operator()(Range&& range) const {
        auto expected = [](const auto& val) {return val.has_value();};
        auto unexpected = [](const auto& val) {return !val.has_value();};
        auto expected_value = [](const auto& val) { return val.value(); };
        auto unexpected_value = [](const auto& val) { return val.error(); };

        return std::make_pair(
            range | Filter(unexpected) | Transform(unexpected_value),
            range | Filter(expected) | Transform(expected_value)
        );
    }
};