#pragma once

#include <optional>

#include "Range.h"
#include "Transform.h"

template <typename Key, typename Value>
struct KV {
    using key_type = Key;
    using value_type = Value;
	Key key;
	Value value;

    bool operator==(const KV& other) const {
        return key == other.key && value == other.value;
    }

    bool operator!=(const KV& other) const {
        return !(*this == other);
    }
};

template <typename Base, typename Joined>
struct JoinResult {
	Base base;
	std::optional<Joined> joined;

    bool operator==(const JoinResult& other) const {
        return base == other.base && joined == other.joined;
    }

    bool operator!=(const JoinResult& other) const {
        return !(*this == other);
    }
};

template<class LRange, class RRange>
class JoinIteratorKV {
public:
    using left_value_type  = typename LRange::value_type::value_type;
    using right_value_type = typename RRange::value_type::value_type;

    JoinIteratorKV(LRange::const_iterator it1, const RRange& rrange)
    : it(it1)
    , right_range(rrange) {}

    JoinIteratorKV& operator++() {
        ++it;
        return *this;
    }

    JoinIteratorKV operator++(int) {
        auto copy(*this);
        ++it;
        return copy;
    }

    JoinResult<left_value_type, right_value_type> operator*() const {
        for (auto c : right_range) {
            if (c.key == (*it).key) {
                return JoinResult<left_value_type, right_value_type>{(*it).value, c.value};
            }
        }
        return JoinResult<left_value_type, right_value_type>{(*it).value, std::nullopt};
    }

    bool operator==(const JoinIteratorKV& other) const {
        return it == other.it;
    }

    bool operator!=(const JoinIteratorKV& other) const {
        return it != other.it;
    }
private:
    typename LRange::const_iterator it;
    RRange right_range;
};

template<class LRange, class RRange>
class JoinKVRange {
public:

    using const_iterator = JoinIteratorKV<LRange, RRange>;
    using left_value_type  = typename LRange::value_type::value_type;
    using right_value_type = typename RRange::value_type::value_type;
    using value_type = JoinResult<left_value_type, right_value_type>;

    JoinKVRange(const LRange& lrange, const RRange& rrange)
    : lrange_(lrange)
    , rrange_(rrange) {}

    const_iterator begin() const {
        return const_iterator(lrange_.begin(), rrange_);
    }

    const_iterator end() const {
        return const_iterator(lrange_.end(), rrange_);
    }

private:
    LRange lrange_;
    RRange rrange_;
};

template<RangeSatisfiable Range>
class JoinKVObject : public Pipe {
public:
    JoinKVObject(const Range& range)
    : range_(range)
    , using_key_(false) {}

    template<RangeSatisfiable LRange>
    auto operator()(const LRange& lrange) const {
        return JoinKVRange<LRange, Range>(lrange, range_);
    }
private:
    bool using_key_;
    Range range_;
};

template<RangeSatisfiable Range, class Key1, class Key2>
class JoinComparatorObject : public Pipe {
public:
    JoinComparatorObject(const Range& range, const Key1& key1, const Key2& key2) 
    : range_(range)
    , key1_(key1)
    , key2_(key2) {}

    template<RangeSatisfiable LRange>
    auto operator()(const LRange& lrange) const {
        auto new_l_range = lrange | Transform(
            [this](const typename LRange::value_type& value) {
                return KV<
                    std::invoke_result_t<Key1, typename LRange::value_type>, 
                    typename LRange::value_type
                >(key1_(value), value);
            }
        );
        auto new_r_range = range_ | Transform(
            [this](const typename Range::value_type& value) {
                return KV<
                    std::invoke_result_t<Key2, typename Range::value_type>, 
                    typename Range::value_type
                >(key2_(value), value);
            }
        );
        return JoinKVRange(new_l_range, new_r_range);
    } 
private:
    Range range_;
    Key1 key1_;
    Key2 key2_;
};


template<RangeSatisfiable Range>
auto Join(const Range& range) {
    return JoinKVObject(range);
}

template<RangeSatisfiable Range, class Key1, class Key2>
auto Join(const Range& range, const Key1& key1, const Key2& key2) {
    return JoinComparatorObject<Range, Key1, Key2>(range, key1, key2);
}
