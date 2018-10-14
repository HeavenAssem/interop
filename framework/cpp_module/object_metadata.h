//
// Created by islam on 20.06.17.
//

#pragma once

#include "function_metadata.h"
#include <memory>

namespace interop {
struct object_metadata_t {
    std::string name;
    destructor_metadata_t destructor;
    std::vector<constructor_metadata_t> constructors;
    std::vector<variable_metadata_t> fields;
    std::vector<function_metadata_t> methods;

    explicit object_metadata_t(std::string name)
      : name(std::move(name))
    {}
};
} // namespace interop
