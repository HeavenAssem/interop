#pragma once

#include <optional>

namespace interop::utils {

template <typename MAP>
typename MAP::mapped_type * find_ptr(MAP & map, const typename MAP::key_type & key)
{
    if (auto it = map.find(key); it != map.end()) {
        return &it->second;
    }

    return nullptr;
}

template <typename MAP>
std::optional<typename MAP::mapped_type> find_opt(MAP & map, const typename MAP::key_type & key)
{
    if (auto ptr = find_ptr(map, key)) {
        return *ptr;
    }

    return std::nullopt;
}

} // namespace interop::utils
