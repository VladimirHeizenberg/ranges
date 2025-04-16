#pragma once
#include <type_traits>

#include "Range.h"

template<class  Range, class Predicate>
class FilterIterator {
public:
    using ClearRange = std::remove_reference_t<Range>;
    using value_type        = typename ClearRange::value_type;
    using reference         = value_type&;
    using const_reference   = const value_type&;
    using iterator_category = std::input_iterator_tag;

    FilterIterator(typename ClearRange::iterator it,
                   typename ClearRange::iterator end,
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

    auto operator*() {
        return *it_;
    }

    bool operator==(const FilterIterator& other) const {
        return it_ == other.it_;
    }

    bool operator!=(const FilterIterator& other) const {
        return !(*this == other);
    }

private:
    typename ClearRange::iterator it_;
    typename ClearRange::iterator end_;
    Predicate func_;
};


template <typename T>
struct range_value {
    using type = std::iter_value_t<decltype(std::begin(std::declval<T&>()))>;
};

template <typename T>
using range_value_t = typename range_value<T>::type;

template<class  Range, class Predicate>
class FilterRange {
public:
    using value_type = range_value_t<Range>;
    using iterator = FilterIterator<Range, Predicate>;

    FilterRange(Range&& range, const Predicate& pred)
    : range_(std::forward<Range>(range))
    , pred_(pred) {}

    iterator begin() {
        return iterator(range_.begin(), range_.end(), pred_);
    }

    iterator end() {
        return iterator(range_.end(), range_.end(), pred_);
    }
private:
    Range range_;
    Predicate pred_;
};

template<class Predicate>
class Filter : public Pipe {
public:
    template<class  Range>
    auto operator()(Range&& range) const {
        using CleanRange = std::remove_reference_t<Range>;
        return FilterRange<Range, Predicate>(std::forward<Range>(range), func_);
    }

    Filter(const Predicate& func)
        : func_(func) {}
private:
    Predicate func_;
};