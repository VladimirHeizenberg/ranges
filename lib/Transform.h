#pragma once

#pragma once
#include <type_traits>

#include "Range.h"

template<class Range, class Predicate>
class TransformIterator {
public:
    using value_type = std::invoke_result_t<Predicate, typename Range::value_type>;

    TransformIterator(Range::const_iterator it,
                      Range::const_iterator end,
                      Predicate func)
        : it_(it)
        , func_(func) {}

    TransformIterator& operator++() {
        ++it_;
        return *this;
    }

    TransformIterator operator++(int) {
        auto copy(*this);
        ++(*this);
        return copy;
    }

    value_type operator*() {
        return func_(*it_);
    }

    bool operator==(const TransformIterator& other) const {
        return it_ == other.it_;
    }

    bool operator!=(const TransformIterator& other) const {
        return !(*this == other);
    }

private:
    typename Range::const_iterator it_;
    Predicate func_;
};

template<class Range, class Predicate>
class TransformRange {
public:
    using value_type = std::invoke_result_t<Predicate, typename Range::value_type>;
    using const_iterator = TransformIterator<Range, Predicate>;

    TransformRange(const Range& range, const Predicate& pred)
        : range_(range)
        , pred_(pred) {}

    const_iterator begin() const {
        return const_iterator(range_.begin(), range_.end(), pred_);
    }

    const_iterator end() const {
        return const_iterator(range_.end(), range_.end(), pred_);
    }
private:
    Range range_;
    Predicate pred_;
};

template<class Predicate>
class Transform : public Pipe {
public:
    template<class Range>
    TransformRange<Range, Predicate> operator()(const Range& range) const {
        return TransformRange(range, func_);
    }

    Transform(const Predicate& func)
        : func_(func) {}
private:
    Predicate func_;
};