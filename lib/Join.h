#pragma once

#include <optional>

#include "Range.h"

template <typename Key, typename Value>
struct KV {
    using key_type = typename Key;
    using value_type = typename Value;
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
        for (auto& c : right_range) {
            if (c.key == (*it).key) {
                return JoinResult<left_value_type, right_value_type>{(*it).value, c.value};
            }
        }
        return JoinResult<left_value_type, right_value_type>{(*it).value, std::nullopt};
    }

    bool operator==(const JoinIteratorKV& other) {
        return it == other.it;
    }

    bool operator!=(const JoinIteratorKV& other) {
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

template<class Range>
class Join : public Pipe {
public:
    Join(const Range& range)
    : range_(range) {}

    template<class LRange>
    auto operator()(const LRange& lrange) const {
        return JoinKVRange<LRange, Range>(lrange, range_);
    }
private:
    Range range_;
};



// template<class Range, class Key1, class Key2>
// class Join : public Pipe {
//     Join(const Range& range)
//     : range_(range) {}

//     template<class LRange>
//     auto operator()(const LRange& range) {
//         return ;
//     }
// private:
//     Range range_;
//     Key1 key1_;
//     Key2 key2_;
// };
