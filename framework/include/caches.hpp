#pragma once

#include "declarations.h"
#include <string_view_cache.hpp>

namespace interop {

template <typename T>
struct named_entity_ptr_key_getter_t {
    static const std::string & get_persistent_key(const std::shared_ptr<T> & ptr)
    {
        return ptr->name();
    }
};

template <typename T>
using named_entity_cache_t =
    string_view_cache<std::shared_ptr<T>, named_entity_ptr_key_getter_t<T>>;

} // namespace interop
