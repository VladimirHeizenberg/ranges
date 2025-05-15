#pragma once

#include "Range.h"
#include <string>

namespace details {

template<class RangeIterator>
class SplitIterator {
private:
    RangeIterator it_;
};

template<class RangeIterator>
class ConstSplitIterator {
    
};

template<class Range>
class SplitRange {
public:
    using value_type = typename std::string;
    using iterator = SplitIterator<;
    using const_iterator = SplitIterator<Range>
    template<class LRange>
    SplitRange(LRange&& range)
    : range_(std::forward<LRange>(range)) {}


private:
    RangeStorage<Range> range_;
};

}  // namespace details 

class Split : public Pipe {
public:
    Split(std::string delims)
    : delims_(std::move(delims)) {}

    template<class Range>
    auto operator()(Range&& range) {

    }
private:
    std::string delims_;
};