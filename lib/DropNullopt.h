#pragma once

#include <functional>
#include <optional>

#include "Range.h"
#include "Filter.h"

class DropNullopt : public Pipe {
public:
    template<class Range>
    auto operator()(const Range& range) const {
        using T = typename Range::value_type;
        static_assert(std::is_same_v<T, std::optional<typename T::value_type>>, 
                      "Only std::optional<T> is allowed to be used with DropNullopt");

        auto predicate = [](const std::optional<typename T::value_type>& val) {
            return val.has_value();
        };

        return FilterRange(range, predicate);
    }
};