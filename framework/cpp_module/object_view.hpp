#pragma once
//
// Created by islam on 26.10.17.
//

#include "declarations.h"
#include "field_view.hpp"
#include "object_metadata.h"
#include "object_mixin.hpp"

namespace interop {
class object_view_t: public property_view_t, public internals::object_interface_t {
  public:
    // FIXME: GTHO
    static object_ptr_t create(void * native_object, const object_metadata_t &);

    using internals::object_interface_t::get;

    virtual ~object_view_t() = default;
};
} // namespace interop
