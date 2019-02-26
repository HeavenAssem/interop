#pragma once

#include "invariant.h"

#include <unordered_map>
#include <string_view>

namespace interop {

template <typename Value>
struct default_key_getter {
    static const std::string & get_persistent_key(const Value& value) {
        return value.name;
    }
};

// No heterogeneous lookup on unordered containers, C++? Okay then...
template <typename Value, typename KeyGetter = default_key_getter<Value>>
struct string_view_cache : public std::unordered_map<std::string_view, Value> {
    template <typename Handler>
    const Value & get(const std::string_view& key, const Handler& fetch) {
        // TODO: try to avoid default construction
        auto [it, is_new] = this->emplace(key, Value{});
        auto& [cached_key, cached_value] = *it;
        if (is_new) {
            cached_value = fetch(key);
            const std::string& persistent_key = KeyGetter::get_persistent_key(cached_value);
            interop_invariant_m(cached_key == persistent_key, "keys mismatch");
            // Because we don't know what passed name's lifetime is, we bind it to persistent key
            const_cast<std::string_view &>(cached_key) = persistent_key;
        }
        return cached_value;
    }
};

}
