#pragma once

#include "caches.hpp"
#include "object_metadata.h"

#include <object_view.hpp>

namespace interop {

class native_object_t final: public internals::object_caching_layer_t<object_view_t> {
    const object_metadata_t & metadata;
    void * pointer;

  public:
    native_object_t(void * pointer, const object_metadata_t & metadata)
      : metadata(metadata)
      , pointer(pointer)
    {}

    const std::string & name() const override;
    ~native_object_t();

  private:
    function_ptr_t fetch_function(const std::string_view & name) const override;
    field_ptr_t fetch_field(const std::string_view & name) const override;
};
} // namespace interop
