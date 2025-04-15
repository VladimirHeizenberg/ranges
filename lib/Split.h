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
        return std::string()
    }
private:
    std::istream_iterator it;
};

class Split : public Pipe {
public:
    Split(std::string&& delims)
    : delims_(std::move(delims)) {}

    Split(const std::string& delims)
    : delims_(delims) {}

    template<class Range>
    SplitRange operator()(const Range& range) {

    }
private:
    std::string delims_;
};



class Iterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::string;
    Iterator(base_iterator outer, base_iterator end, const std::string& delims)
        : outer_(outer), end_(end), delims_(delims) {
        if (outer_ != end_) {
            line_ = *outer_;
            advance_token();
        }
    }
    std::string operator*() const {
        return current_token_;
    }
    Iterator& operator++() {
        advance_token();
        return *this;
    }
    bool operator!=(const Iterator& other) const {
        return outer_ != other.outer_ || token_pos_ != std::string::npos;
    }
private:
    base_iterator outer_;
    base_iterator end_;
    std::string delims_;
    std::string line_;
    size_t token_start_ = 0;
    size_t token_pos_ = std::string::npos;
    std::string current_token_;
    void advance_token() {
        while (true) {
            if (token_pos_ == std::string::npos) {
                // нужно перейти к следующей строке
                ++outer_;
                if (outer_ == end_) return;
                line_ = *outer_;
                token_start_ = 0;
            }
            token_start_ = line_.find_first_not_of(delims_, token_start_);
            if (token_start_ == std::string::npos) {
                token_pos_ = std::string::npos;
                continue; // пропускаем пустую строку
            }
            token_pos_ = line_.find_first_of(delims_, token_start_);
            current_token_ = line_.substr(token_start_, token_pos_ - token_start_);
            token_start_ = token_pos_;
            return;
        }
    }
};


template<class Range>
class SplitRange {
public:
    using base_iterator = typename Range::const_iterator;
    using value_type = std::string;

    SplitRange(const Range& range, const std::string& delims)
        : range_(range), delims_(delims) {}

    Iterator begin() const {
        return Iterator(range_.begin(), range_.end(), delims_);
    }

    Iterator end() const {
        return Iterator(range_.end(), range_.end(), delims_);
    }

private:
    const Range& range_;
    std::string delims_;
};
