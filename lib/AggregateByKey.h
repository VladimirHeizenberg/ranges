#pragma once

#include <type_traits>
#include <map>

#include "Range.h"

template<class Key, class Value>
class AggregateByKeyRange {
public: 
    using value_type = std::pair<Key, Value>;
    using const_iterator = std::map<Key, Value>::const_iterator;
    AggregateByKeyRange(std::map<Key, Value>&& aggregated)
    : aggregated_(std::forward<std::map<Key, Value>>(aggregated)) {}

    const_iterator begin() const {
        return aggregated_.begin();
    }
    
    const_iterator end() const {
        return aggregated_.end();
    }
private:
    std::map<Key, Value> aggregated_;
};

template<class Right, class Aggregation, class Key>
class AggregateByKey : public Pipe {
public:
    AggregateByKey(
        Right&& right,
        const Aggregation& aggregation,
        const Key& key
    ) : right_(std::forward<Right>(right))
      , aggregation_(aggregation)
      , key_(key) {}

    template<RangeSatisfiable Range>
    auto operator()(const Range& range) const {
        using key_type = std::decay_t<decltype(key_(*range.begin()))>;
        std::map<key_type, Right> aggregated;
        for (auto& elem : range) {
            auto t = key_(elem);
            if (!aggregated.contains(t)) {
                aggregated[t] = right_;
            }
            aggregation_(elem, aggregated[t]);
        }
        return AggregateByKeyRange<key_type, Right>(std::move(aggregated));
    }

private:
    Right right_;
    Aggregation aggregation_;
    Key key_;
};