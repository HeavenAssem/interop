//
// Created by islam on 07.05.17.
//

#pragma once

#include "module_context.h"
#include "module_metadata.h"

namespace interop {
typedef module_metadata_t (*describe_module_function)();
typedef void (*initialize_module_function)(module_context_t &);
typedef void (*register_module_function)();
typedef void (*shutdown_module_function)();
} // namespace interop
