#pragma once

#pragma once
#include <type_traits>

#include "Range.h"

template<class  Range, class TransformationFunction>
class TransformIterator {
public:
    using value_type        = std::invoke_result_t<TransformationFunction, typename Range::value_type>;
    using reference         = value_type&;
    using const_reference   = const reference;
    using iterator_category = std::input_iterator_tag;

    TransformIterator(typename Range::iterator it,
                      typename Range::iterator end,
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
    typename Range::iterator it_;
    TransformationFunction func_;
};

template<class  Range, class TransformationFunction>
class TransformRange {
public:
    using ClearRange = std::remove_reference_t<Range>;
    using value_type = std::invoke_result_t<TransformationFunction, typename ClearRange::value_type>;
    using iterator = TransformIterator<ClearRange, TransformationFunction>;

    TransformRange(Range&& range, const TransformationFunction& pred)
        : range_(range)
        , pred_(pred) {}

    iterator begin() {
        return iterator(range_.begin(), range_.end(), pred_);
    }

    iterator end() {
        return iterator(range_.end(), range_.end(), pred_);
    }
private:
    Range range_;
    TransformationFunction pred_;
};

template<class  TransformationFunction>
class Transform : public Pipe {
public:
    template<class  Range>
    auto operator()(Range&& range) const {
        return TransformRange<Range, TransformationFunction>(std::forward<Range>(range), func_);
    }

    Transform(TransformationFunction func)
        : func_(std::move(func)) {}
private:
    TransformationFunction func_;
};