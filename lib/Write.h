#pragma once
#include <sstream>

#include "Range.h"

class Write : public Pipe {
public:
    Write(std::ostream& out, const char delimiter)
        : out_(out), delimiter_(delimiter) {}
  
    template<class Range>
    Range&& operator()(Range&& range) const {
        for (const auto& item : range) {
            out_ << item << delimiter_;
        }
        return std::forward<Range>(range);
    }
  
private:
    std::ostream& out_;
    const char delimiter_;
};