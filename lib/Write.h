#pragma once
#include <sstream>

#include "Range.h"

class Write : public Pipe {
public:
    Write(std::ostream& out, const char delimiter)
        : out_(out), delimiter_(delimiter) {}
  
    template<class Range>
    Range operator()(const Range& range) const {
        for (auto& item : range) {
            out_ << item << delimiter_;
        }
        return range;
    }
  
private:
    std::ostream& out_;
    const char delimiter_;
};