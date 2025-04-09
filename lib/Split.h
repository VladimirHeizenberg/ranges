#pragma once

#include <string>
#include <iterator>

#include "Range.h"

template<class Range>
class SplitIterator {
public:
    using value_type = typename std::string;
    using size_type = size_t;

    value_type operator*() {
        return (*it).substr()
    }
private:
    std::istream_iterator it;
};

class SplitRange {

};

class Split : public Pipe {
    template<class Range>
    SplitRange operator()(const Range& range) {

    }
};