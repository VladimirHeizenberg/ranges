#pragma once
#include "Range.h"

template<class Container>
class AsDataFlow : public Pipe {
public:
    using value_type = typename Container::value_type;
    using iterator = typename Container::iterator;

    AsDataFlow(Container& container) 
    : begin_(container.begin())
    , end_(container.end()) {}

    iterator begin() const {
        return begin_;
    }

    iterator end() const {
        return end_;
    }

private:
    iterator begin_;
    iterator end_;
};