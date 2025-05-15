#pragma once

#include <type_traits>
#include <concepts>

class Pipe {};

template<typename T>
using RangeStorage = 
    std::conditional_t<
        std::is_lvalue_reference_v<T>, 
        std::remove_reference_t<T>&, 
        std::remove_reference_t<T>
    >;

template<class L, class R> 
requires (std::is_base_of_v<Pipe, R>)
auto operator|(L&& left_range, R&& adapter) {
    return adapter(std::forward<L>(left_range));
}