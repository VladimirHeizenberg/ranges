#pragma once
#include <type_traits>

#include "Range.h"

template<RangeSatisfiable Range, class Predicate>
class FilterIterator {
public:
    using value_type        = typename Range::value_type;
    using reference         = value_type&;
    using const_reference   = const reference;
    using iterator_category = std::input_iterator_tag;

    FilterIterator(Range::const_iterator it,
        Range::const_iterator end,
        Predicate func)
        : it_(it)
        , end_(end)
        , func_(func) {
        if (it_ != end_ && !func_(*it_)) {
            ++(*this);
        }
    }

    FilterIterator& operator++() {
        ++it_;
        while (it_ != end_ && !func_(*it_)) {
            ++it_;
        }
        return *this;
    }

    FilterIterator operator++(int) {
        auto copy(*this);
        ++(*this);
        return copy;
    }

    const_reference operator*() {
        return *it_;
    }

    bool operator==(const FilterIterator& other) const {
        return it_ == other.it_;
    }

    bool operator!=(const FilterIterator& other) const {
        return !(*this == other);
    }

private:
    typename Range::const_iterator it_;
    typename Range::const_iterator end_;
    Predicate func_;
};

template<RangeSatisfiable Range, class Predicate>
class FilterRange {
public:
    using value_type = Range::value_type;
    using const_iterator = FilterIterator<Range, Predicate>;

    FilterRange(const Range& range, const Predicate& pred)
        : begin_(range.begin())
        , end_(range.end())
        , pred_(pred) {}

    const_iterator begin() const {
        return const_iterator(begin_, end_, pred_);
    }

    const_iterator end() const {
        return const_iterator(end_, end_, pred_);
    }
private:
    Range::const_iterator begin_;
    Range::const_iterator end_;
    Predicate pred_;
};

template<class Predicate>
class Filter : public Pipe {
public:
    template<RangeSatisfiable Range>
    FilterRange<Range, Predicate> operator()(const Range& range) const {
        return FilterRange(range, func_);
    }

    Filter(const Predicate& func)
        : func_(func) {}
private:
    Predicate func_;
};