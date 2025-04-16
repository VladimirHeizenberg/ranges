#pragma once

#include <type_traits>
#include <map>

#include "Range.h"

template<class Key, class Value>
class AggregateByKeyRange {
public: 
    using value_type = std::pair<Key, Value>;
    using iterator = std::map<Key, Value>::iterator;

    AggregateByKeyRange(std::map<Key, Value>&& aggregated)
    : aggregated_(std::move(aggregated)) {}

    AggregateByKeyRange(const std::map<Key, Value>& aggregated)
    : aggregated_(aggregated) {}

    iterator begin() {
        return aggregated_.begin();
    }
    
    iterator end() {
        return aggregated_.end();
    }
private:
    std::map<Key, Value> aggregated_;
};

template<class Value, class AggregationFunctionType, class KeyFunction>
class AggregateByKey : public Pipe {
public:
    AggregateByKey(
        const Value& right,
        const AggregationFunctionType& aggregation,
        const KeyFunction& key
    ) : initial_right_value_(right)
      , aggregation_function_(aggregation)
      , key_(key) {}

    template<class  Range>
    auto operator()(Range&& range) const {
        using key_type = std::decay_t<decltype(key_(*range.begin()))>;
        std::map<key_type, Value> aggregated;
        for (auto& elem : range) {
            auto t = key_(elem);
            if (!aggregated.contains(t)) {
                aggregated[t] = initial_right_value_;
            }
            aggregation_function_(elem, aggregated[t]);
        }
        return AggregateByKeyRange<key_type, Value>(std::move(aggregated));
    }

private:
    Value initial_right_value_;
    AggregationFunctionType aggregation_function_;
    KeyFunction key_;
};