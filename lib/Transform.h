#pragma once

#pragma once
#include <type_traits>

#include "Range.h"

template<RangeSatisfiable Range, class TransformationFunction>
class TransformIterator {
public:
    using value_type        = std::invoke_result_t<TransformationFunction, typename Range::value_type>;
    using reference         = value_type&;
    using const_reference   = const reference;
    using iterator_category = std::input_iterator_tag;

    TransformIterator(Range::const_iterator it,
                      Range::const_iterator end,
                      TransformationFunction func)
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

    const_reference operator*() {
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
    TransformationFunction func_;
};

template<RangeSatisfiable Range, class TransformationFunction>
class TransformRange {
public:
    using value_type = std::invoke_result_t<TransformationFunction, typename Range::value_type>;
    using const_iterator = TransformIterator<Range, TransformationFunction>;

    TransformRange(const Range& range, const TransformationFunction& pred)
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
    TransformationFunction pred_;
};

template<class TransformationFunction>
class Transform : public Pipe {
public:
    template<RangeSatisfiable Range>
    TransformRange<Range, TransformationFunction> operator()(const Range& range) const {
        return TransformRange(range, func_);
    }

    Transform(const TransformationFunction& func)
        : func_(func) {}
private:
    TransformationFunction func_;
};