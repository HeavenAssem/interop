#pragma once
//
// Created by islam on 26.10.17.
//

#include "declarations.h"
#include "object_metadata.h"

namespace interop {
class object_view_t {
    const object_metadata_t & metadata;
    void * pointer;

  public:
    object_view_t(void * pointer, const object_metadata_t & metadata)
      : metadata(metadata)
      , pointer(pointer)
    {}

    function_ptr_t function(const std::string & name) const;

    void * get_pointer();

    ~object_view_t();
};
} // namespace interop
