//
// Created by islam on 07.05.17.
//

#pragma once

#include "internals/native_module_data.hpp"

namespace interop {
// TODO: do not use C++ here
struct module_metadata_t: internals::native_module_data_t {
    std::string name;
};
} // namespace interop
