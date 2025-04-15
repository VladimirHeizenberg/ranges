#pragma once

#include <utility>
#include <concepts>

class Pipe {};

template <class T>
concept RangeSatisfiable = requires(const T& a) {
    typename T::const_iterator;
    typename T::value_type;

    { a.begin() } -> std::same_as<typename T::const_iterator>;
    { a.end() } -> std::same_as<typename T::const_iterator>;
};

template<RangeSatisfiable Range, class Adapter> requires std::derived_from<Adapter, Pipe>
auto operator|(const Range& range, const Adapter& pipe) {
    return pipe(range);
}