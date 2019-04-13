//
// Created by islam on 08.04.18.
//

#include "base_module.h"
#include "logger.hpp"

#include <boost/scope_exit.hpp>

#include <cassert>

using namespace std;
using namespace placeholders;

namespace interop {
base_module_t::base_module_t() {}

void base_module_t::assign_id(module_id_t id)
{
    interop_invariant_m(id != unassigned_id_c, "attempting to assign reserved (unassigned) id");
    interop_invariant_m(this->id == unassigned_id_c, "id was already assigned");
    this->id = id;
}

module_id_t base_module_t::get_id() const
{
    interop_invariant_m(this->id != unassigned_id_c, "id wasn't assigned");
    return id;
}

base_module_t::classes_sequence_t base_module_t::iterate_classes() const
{
    return [this, class_id = class_id_t{id, {}}]() mutable -> classes_sequence_t::value_t {
        BOOST_SCOPE_EXIT(&class_id) { ++class_id.local_id; }
        BOOST_SCOPE_EXIT_END

        if (class_id.local_id == metadata.types.size()) {
            return {};
        }

        return {{class_id, metadata.types[class_id.local_id]}};
    };
}

const object_metadata_t & base_module_t::get_object_metadata(class_id_t class_id) const
{
    using namespace std::string_literals;

    interop_invariant_m(class_id.module_id == id,
                        "accessing metadata of class from another module");
    interop_invariant_m(class_id.local_id < metadata.types.size(),
                        "invalid class id: " << class_id.local_id << ": out of range");

    return metadata.types[class_id.local_id];
}

const module_metadata_t & base_module_t::get_metadata() const { return metadata; }

function_ptr_t base_module_t::function(const string_view & name)
{
    return cache.get(name, bind(&base_module_t::fetch_function, this, _1));
}

void base_module_t::unload()
{
    interop_logger(log, "unload module '" + name() + "'");

    cache.clear();
}
} // namespace interop
