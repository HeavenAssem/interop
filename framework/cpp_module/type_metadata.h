//
// Created by islam on 27.05.17.
//

#pragma once

#include "type_subsystem/type.h"

#include "type_subsystem/type_id.hpp"

namespace interop {

struct type_metadata_t {
    type_e type;
    std::size_t size;
};

[[maybe_unused]] const type_id_t id;

} // namespace interop
