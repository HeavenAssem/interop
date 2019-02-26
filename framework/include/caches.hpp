#pragma once

#include "declarations.h"
#include <string_view_cache.hpp>

namespace interop
{

struct function_ptr_key_getter_t {
  static const std::string & get_persistent_key(const function_ptr_t&);
};

using function_cache_t = string_view_cache<function_ptr_t, function_ptr_key_getter_t>;

} // interop
