#pragma once

#include <istream>
#include <string>
#include <vector>

#include "Range.h"

template <typename RangeIterator>
class SplitIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type        = std::string;
    using reference         = value_type&;
    using const_reference   = const value_type&;

    SplitIterator(RangeIterator it, RangeIterator end, const std::string& delims)
        : it_(it)
        , end_(end)
        , delims_(delims)
        , end_iter_(false) {
        ++(*this);
    }

    SplitIterator& operator++() {
        word_.clear();
        if (end_iter_) {
            // drop stream
            it_->clear();
            it_->seekg(0);
            ++it_;
            end_iter_ = false;
        }
        if (it_ == end_) {
            end_iter_ = true;
            return *this;
        }
        char ch;
        while (it_->get(ch)) {
            if (delims_.contains(ch)) {
                break;
            }
            word_ += ch;
        }
        if (it_->eof()) end_iter_ = true;
        return *this;
    }


    SplitIterator operator++(int) {
        auto copy(*this);
        ++(*this);
        return copy;
    }

    std::string& operator*() {
        return (word_);
    }

    bool operator==(const SplitIterator& other) const {
        return (it_ == other.it_);
    }

    bool operator!=(const SplitIterator& other) const {
        return !(*this == other);
    }

private:
    RangeIterator it_;
    RangeIterator end_;
    std::string delims_;
    std::string word_;
    bool end_iter_;
};

template <typename Range>
class SplitRange {
public:
    using CleanRange = std::remove_reference_t<Range>;
    using iterator   = SplitIterator<decltype(std::declval<Range&>().begin())>;
    using value_type = std::string;

    template<class R>
    SplitRange(R&& range, std::string delims)
        : range_(std::forward<Range>(range))
        , delims_(std::move(delims)) {}

    iterator begin() const {
        return iterator(range_.begin(), range_.end(), delims_);
    }

    iterator end() const {
        return iterator(range_.end(), range_.end(), delims_);;
    }

private:
    Range range_;
    std::string delims_;
};

class Split : public Pipe {
public:
    Split(std::string delims) : delims_(std::move(delims)) {}

    template <typename Range>
    auto operator()(Range&& range) const {
        using CleanRange = std::remove_reference_t<Range>;
        return SplitRange<CleanRange>(std::forward<Range>(range), delims_);
    }

private:
    std::string delims_;
};