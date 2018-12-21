//
// Created by islam on 27.05.17.
//

#pragma once

#include "type_subsystem/type.h"

namespace interop {

struct type_metadata_t {
    type_e type;
    std::size_t size;
};

struct field_metadata_t: type_metadata_t {
    std::string name;
    void * getter;
    void * setter;
};
} // namespace interop
