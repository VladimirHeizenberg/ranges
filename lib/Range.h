#pragma once

#include <utility>

class Pipe {
    template<class Range, class Adapter>
    friend auto operator|(const Range& range, const Adapter& pipe) {
        return pipe(range);
    }
};