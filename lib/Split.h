#pragma once

#include <istream>
#include <string>
#include <vector>
#include <__filesystem/recursive_directory_iterator.h>

#include "Range.h"

template<class Range>
class SplitIterator {
public:
    using value_type        = std::string;
    using reference         = std::string&;
    using const_reference   = const std::string&;
    using iterator_category = std::input_iterator_tag;

    SplitIterator() = default;

    SplitIterator(typename Range::iterator it1, typename Range::iterator end, std::string delims)
        : it(it1), end_(end), delims_(std::move(delims)) {
        ++(*this);
    }

    SplitIterator& operator++() {
        word_.clear();
        while (it != end_) {
            char ch;
            if (!it->get(ch)) {
                it->clear();
                it->seekg(0);
                ++it;
                return *this;
            }
            if (delims_.contains(ch)) {
                return *this;
            }
            word_ += ch;
        }
        return *this;
    }

    SplitIterator operator++(int) {
        SplitIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    reference operator*() { return word_; }
    std::string operator*() const { return word_; }

    bool operator==(const SplitIterator& other) const {
        return it == other.it && word_ == other.word_;
    }

    bool operator!=(const SplitIterator& other) const {
        return !(*this == other);
    }

private:
    typename Range::iterator it;
    typename Range::iterator end_;
    std::string delims_;
    std::string word_;
};

template<class Range>
class SplitRange {
public:
    using value_type = std::string;
    using iterator = SplitIterator<Range>;

    SplitRange(Range&& range, std::string delims)
    : range_(std::move(range))
    , delims_(std::move(delims)) {}

    iterator begin() {
        return SplitIterator<Range>(range_.begin(), range_.end(), delims_);
    }

    iterator end() {
        return SplitIterator<Range>(range_.end(), range_.end(), delims_);
    }

private:
    Range range_;
    std::string delims_;
};

class Split : public Pipe {
public:
    Split(std::string delims)
    : delims_(std::move(delims)) {}

    template<class Range>
    auto operator()(Range&& range) const {
        return SplitRange(std::forward<Range>(range), delims_);
    }

private:
    std::string delims_;
};
