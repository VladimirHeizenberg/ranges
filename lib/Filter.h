#pragma once

#include "Range.h"

namespace details {

template<class RangeIterator, class Predicate>
class FilterIterator {
public:
    using value_type        = typename RangeIterator::value_type;
    using reference         = value_type&;
    using const_reference   = const value_type&;
    using iterator_category = std::input_iterator_tag;

    FilterIterator(RangeIterator it, RangeIterator end, 
                   const Predicate& predicate)
    : it_(it)
    , end_(end)
    , predicate_(predicate) {
        while (it_ != end_ && !predicate_(*it_)) {
            ++it_;
        }
        if (it_ != end_) {
            current_ = *it_;
        }
    }

    FilterIterator& operator++() {
        ++it_;
        while (it_ != end_ && !predicate_(*it_)) {
            ++it_;
        }
        if (it_ != end_) current_ = *it_;
        return *this;
    }

    FilterIterator operator++(int) {
        auto copy(*this);
        ++(*this);
        return copy;
    }

    reference operator*() {
        return (current_);
    }

    bool operator==(const FilterIterator& other) const {
        return it_ == other.it_;
    }

    bool operator!=(const FilterIterator& other) const {
        return !(*this == other);
    }

private:
    RangeIterator it_;
    RangeIterator end_;
    Predicate predicate_;
    value_type current_;
};

template<class RangeIterator, class Predicate>
class ConstFilterIterator {
public:
    using value_type        = typename RangeIterator::value_type;
    using reference         = value_type&;
    using const_reference   = const value_type&;
    using iterator_category = std::input_iterator_tag;

    ConstFilterIterator(RangeIterator it, RangeIterator end,
                        const Predicate& predicate)
    : it_(it)
    , end_(end)
    , predicate_(predicate) {
        while (it_ != end_ && !predicate_(*it_)) {
            ++it_;
        }
        if (it_ != end_) current_ = *it_;
    }

    ConstFilterIterator& operator++() {
        ++it_;
        while (it_ != end_ && !predicate_(*it_)) {
            ++it_;
        }
        if (it_ != end_) current_ = *it_;
        return *this;
    }

    ConstFilterIterator operator++(int) {
        auto copy(*this);
        ++(*this);
        return copy;
    }

    const_reference operator*() const {
        return (current_);
    }

    bool operator==(const ConstFilterIterator& other) const {
        return it_ == other.it_;
    }

    bool operator!=(const ConstFilterIterator& other) const {
        return !(*this == other);
    }

private:
    RangeIterator it_;
    RangeIterator end_;
    Predicate predicate_;
    value_type current_;
};

template<class Range, class Predicate>
class FilterRange {
public:
    using CleanRange = std::remove_reference_t<Range>;
    using value_type = typename CleanRange::value_type;
    using iterator = FilterIterator<decltype(std::declval<Range&>().begin()), Predicate>;
    using const_iterator = ConstFilterIterator<decltype(std::declval<const Range&>().begin()), Predicate>;

    template<class LRange, class P>
    FilterRange(LRange&& range, const P& predicate)
        : range_(std::forward<LRange>(range))
        , predicate_(predicate) {}

    iterator begin() {
        return iterator(range_.begin(), range_.end(), predicate_);
    }

    iterator end() {
        return iterator(range_.end(), range_.end(), predicate_);
    }

    const_iterator begin() const {
        return const_iterator(range_.begin(), range_.end(), predicate_);
    }

    const_iterator end() const {
        return const_iterator(range_.end(), range_.end(), predicate_);
    }

private:
    Predicate predicate_;
    RangeStorage<Range> range_;
};
} // namespace details

template<class Predicate>
class Filter: public Pipe {
public:

    Filter(Predicate predicate)
    : predicate_(std::move(predicate)) {}

    template<class Range>
    auto operator()(Range&& range) const {
        return details::FilterRange<Range, Predicate>(
            std::forward<Range>(range), predicate_
        );
    }
private:
    Predicate predicate_;
};