#pragma once

#include "../cpp_module/declarations.h"

namespace interop {
class module_view_t;
class base_module_t;

class platform_t;
class platform_factory_t;
class platform_function_t;

struct platform_configuration_t;

using module_ptr              = std::unique_ptr<base_module_t>;
using platform_ptr            = std::shared_ptr<platform_t>;
using platform_factory_ptr    = std::shared_ptr<platform_factory_t>;
using platform_function_ptr_t = std::shared_ptr<platform_function_t>;
} // namespace interop
