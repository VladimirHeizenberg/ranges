#pragma once

#include <utility>
#include <concepts>

class Pipe {};

template<class  Range, class Adapter> requires std::derived_from<Adapter, Pipe>
auto operator|(Range&& range, const Adapter& pipe) {
    return pipe(std::forward<Range>(range));
}