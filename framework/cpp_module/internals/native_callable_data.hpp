#pragma once

#include <function_metadata.h>

#include <optional>

namespace interop {
namespace internals {
/**
 * @brief data necessary to perform zero overhead strict call (native to native)
 *
 */
struct native_callable_data_t {
    using metadata_checker_t = void (*)(const std::string & name, const native_callable_data_t &);
    void * pointer           = nullptr;
    void * context           = nullptr;
    std::vector<type_metadata_t> arguments; // FIXME: unnecessary copy from metadata
    type_metadata_t return_type;
    mutable std::vector<metadata_checker_t> metadata_checkers = {};
};

using native_callable_data_opt_t = std::optional<native_callable_data_t>;
} // namespace internals
} // namespace interop
