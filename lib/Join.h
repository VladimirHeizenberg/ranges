#pragma once

#include <optional>

#include "Range.h"
#include "Transform.h"

template <typename Key, typename Value>
struct KV {
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
    using CleanRRange           = std::remove_reference_t<RRange>;

    using left_value_type       = typename LRange::value_type::value_type;
    using right_value_type      = typename CleanRRange::value_type::value_type;

    JoinIteratorKV(LRange::iterator it1, const RRange& rrange)
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

    JoinResult<left_value_type, right_value_type> operator*() {
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
    typename LRange::iterator it;
    RRange right_range;
};

template<class  LRange, typename RRange>
class JoinKVRange {
public:

    using iterator              = JoinIteratorKV<LRange, RRange>;
    using CleanRRange           = std::remove_reference_t<RRange>;

    using raw_left_value_type   = typename LRange::value_type;
    using raw_right_value_type  = typename CleanRRange::value_type;

    using left_value_type       = typename raw_left_value_type::value_type;
    using right_value_type      = typename raw_right_value_type::value_type;

    using value_type = JoinResult<left_value_type, right_value_type>;

    JoinKVRange(const LRange& lrange, const RRange& rrange)
    : lrange_(lrange)
    , rrange_(rrange) {}

    iterator begin() {
        return iterator(lrange_.begin(), rrange_);
    }

    iterator end() {
        return iterator(lrange_.end(), rrange_);
    }

private:
    LRange lrange_;
    RRange rrange_;
};

template<class  Range>
class JoinKVObject : public Pipe {
public:
    JoinKVObject(const Range& range)
    : using_key_(false)
    , range_(range) {}

    template<class LRange>
    auto operator()(LRange&& lrange) const {
        using CleanLRange = std::remove_reference_t<LRange>;

        return JoinKVRange<CleanLRange, Range>(std::forward<LRange>(lrange), range_);
    }
private:
    bool using_key_;
    Range range_;
};

template<class Range, class Key1, class Key2>
class JoinComparatorObject : public Pipe {
public:
    JoinComparatorObject(Range&& range, const Key1& key1, const Key2& key2)
    : range_(std::forward<Range>(range))
    , key1_(key1)
    , key2_(key2) {}

    template<class LRange>
    auto operator()(const LRange& lrange)const {
        using ClearRange = std::remove_reference_t<Range>;

        auto new_l_range = lrange | Transform(
            [this](const typename LRange::value_type& value) {
                return KV<
                    std::invoke_result_t<Key1, typename LRange::value_type>, 
                    typename LRange::value_type
                >(key1_(value), value);
            }
        );
        auto new_r_range = range_ | Transform(
            [this](const auto& value) {
                return KV<
                    std::invoke_result_t<Key2, typename ClearRange::value_type>,
                    typename ClearRange::value_type
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


template<class Range>
auto Join(Range&& range) {
    return JoinKVObject<Range>(range);
}

template<class Range, class Key1, class Key2>
auto Join(Range&& range, const Key1& key1, const Key2& key2) {
    return JoinComparatorObject<Range, Key1, Key2>(range, key1, key2);
}