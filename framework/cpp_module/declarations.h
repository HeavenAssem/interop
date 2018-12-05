#pragma once

#include <any.h>

#include <memory>
#include <string>
#include <vector>

namespace interop {
class object_view_t;
class function_view_t;
class platform_function_t;

using object_ptr            = std::shared_ptr<object_view_t>;
using function_ptr_t        = std::shared_ptr<function_view_t>;
using platform_function_ptr = std::shared_ptr<platform_function_t>;

using val_t      = interop::any_basic_t<sizeof(std::string)>;
using arg_pack_t = std::vector<val_t>;
} // namespace interop
