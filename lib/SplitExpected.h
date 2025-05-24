#pragma once

#include <expected>

#include "Range.h"
#include "Filter.h"
#include "Transform.h"
#include "AsVector.h"


class SplitExpected : public Pipe {
public:
    template<class Range>
    auto operator()(const Range& range) const {
        auto expected_value = [](const auto& val) { return val.value(); };
        auto unexpected_value = [](const auto& val) { return val.error(); };

        auto a = range | Filter([](const auto& val) { return !val.has_value(); }) | Transform(unexpected_value);
        auto b = range | Filter([](const auto& val) { return val.has_value(); }) | Transform(expected_value);

        return std::make_pair(a, b);
    }
    
};