#pragma once

#include "declarations.h"

namespace interop {
val_t ffi_call(const function_view_t &, arg_pack_t);
}
