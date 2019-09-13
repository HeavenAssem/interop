#pragma once

#include "declarations.h"
#include "type_metadata.h"

#include <optional>

namespace interop::internals {

using dynamic_getter_t = val_t (*)(const void *);
using dynamic_setter_t = void (*)(void *, val_t);

struct native_field_data_t: type_metadata_t {
    void * getter;
    dynamic_getter_t dynamic_getter;
    void * setter                   = nullptr;
    dynamic_setter_t dynamic_setter = nullptr;
};

struct native_field_runtime_data_t {
    const native_field_data_t & meta;
    void * object;
};

using native_field_runtime_data_opt_t = std::optional<native_field_runtime_data_t>;

} // namespace interop::internals
