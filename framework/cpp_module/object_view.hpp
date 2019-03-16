#pragma once
//
// Created by islam on 26.10.17.
//

#include "declarations.h"
#include "object_metadata.h"

namespace interop {
class object_view_t {
  public:
    static object_ptr_t create(void * native_object, const object_metadata_t &);

    virtual const std::string & name() const                             = 0;
    virtual function_ptr_t function(const std::string_view & name) const = 0;
    virtual ~object_view_t()                                             = default;
};
} // namespace interop
