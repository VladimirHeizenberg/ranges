#pragma once
#include "Range.h"

template<RangeSatisfiable Container>
class AsDataFlow : public Pipe {
public:
    using value_type = typename Container::value_type;
    using const_iterator = typename Container::const_iterator;

    AsDataFlow(const Container& container) 
    : begin_(container.begin())
    , end_(container.end()) {}

    const_iterator begin() const {
        return begin_;
    }

    const_iterator end() const {
        return end_;
    }

private:
    const_iterator begin_;
    const_iterator end_;
};