#pragma once

#include "value.h"

#include <memory>
#include <vector>

namespace interop {
class object_view_t;
struct function_view_t;
class platform_function_t; // shouldn't be here

using object_ptr_t            = std::unique_ptr<object_view_t>;
using function_ptr_t          = std::shared_ptr<function_view_t>;
using platform_function_ptr_t = std::shared_ptr<platform_function_t>;

using val_t      = value_t;
using arg_pack_t = std::vector<val_t>;
} // namespace interop
