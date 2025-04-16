#pragma once
#include "Range.h"
#include "Write.h"

class Out : public Pipe {
public:
    Out(std::ostream& out)
    : out_(out){}

    template<class Range>
    Range& operator()(Range&& range) const {
        for (auto& item : range) {
            out_ << item << '\n';
        }
        return range;
    }
  
private:
    std::ostream& out_;
};
