//
// Created by islam on 08.04.18.
//

#include "internal_module.hpp"
#include "logger.hpp"

#include <cassert>

using namespace std;
using namespace placeholders;

namespace interop {

void internal_module_t::assign_id(module_id_t id)
{
    interop_invariant_m(id != unassigned_id_c, "attempting to assign reserved (unassigned) id");
    interop_invariant_m(next_class_id.module_id == unassigned_id_c, "id was already assigned");
    next_class_id.module_id = id;
}

module_id_t internal_module_t::get_id() const
{
    interop_invariant_m(next_class_id.module_id != unassigned_id_c, "id wasn't assigned");
    return next_class_id.module_id;
}

void internal_module_t::unload()
{
    interop_logger(log, "unload module '" + get_name() + "'");
    base_t::unload();
}
} // namespace interop
