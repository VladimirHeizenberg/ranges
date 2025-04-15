#pragma once

#include <utility>
#include <concepts>

template <class T>
concept RangeSatisfiable = requires(const T& a) {
    typename T::const_iterator;

    { a.begin() } -> std::same_as<typename T::const_iterator>;
    { a.end() } -> std::same_as<typename T::const_iterator>;
};

class Pipe {
    template<RangeSatisfiable Range, class Adapter>
    friend auto operator|(const Range& range, const Adapter& pipe) {
        return pipe(range);
    }
};