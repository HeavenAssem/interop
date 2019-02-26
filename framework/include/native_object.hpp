#pragma once

#include "object_metadata.h"
#include "caches.hpp"

#include <object_view.hpp>

namespace interop {


class native_object_t final: public object_view_t {
    const object_metadata_t & metadata;
    void * pointer;
    mutable function_cache_t cache;

  public:
    native_object_t(void * pointer, const object_metadata_t & metadata)
      : metadata(metadata)
      , pointer(pointer)
    {}

    function_ptr_t function(const std::string_view &) const override;

    ~native_object_t();
};
} // namespace interop
