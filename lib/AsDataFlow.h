#pragma once

#include "Range.h"

template<class Container>
class AsDataFlow : public Pipe {
public:
    using value_type = typename Container::value_type;
    using iterator = decltype(std::declval<Container&>().begin());
    using const_iterator = decltype(std::declval<const Container&>().begin());
    
    AsDataFlow(Container&& container)
        : container_(std::move(container)) {}

    AsDataFlow(Container& container)
        : container_(container) {}

    iterator begin() noexcept {
        return container_.begin();
    }

    iterator end() noexcept {
        return container_.end();
    }

    const_iterator begin() const noexcept {
        return container_.begin();
    }

    const_iterator end() const noexcept {
        return container_.end();
    }

private:
    RangeStorage<Container> container_;
};
