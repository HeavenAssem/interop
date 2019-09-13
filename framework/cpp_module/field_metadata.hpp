#pragma once

#include "internals/native_field_data.hpp"

namespace interop {

struct field_metadata_t: internals::native_field_data_t {
    std::string name;
};

} // namespace interop
