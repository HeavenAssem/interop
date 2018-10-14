//
// Created by islam on 07.05.17.
//

#pragma once

#include "object_metadata.h"
#include "type_metadata.h"
#include "version.h"

#include <vector>

namespace interop {
// TODO: do not use C++ here
struct module_metadata_t {
    std::string name;
    version_t version;
    std::vector<function_metadata_t> functions;
    std::vector<object_metadata_t> types;
};
} // namespace interop
