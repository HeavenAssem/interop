#pragma once

#include <object_metadata.h>
#include <type_metadata.h>
#include <version.h>

#include <optional>
#include <vector>

namespace interop::internals {

// TODO: do not use C++ here
struct native_module_data_t {
    version_t version;
    std::vector<function_metadata_t> functions;
    std::vector<object_metadata_t> types;
};

using native_module_data_opt_t = std::optional<native_module_data_t>;

} // namespace interop::internals
