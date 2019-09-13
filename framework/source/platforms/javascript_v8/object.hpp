#pragma once

#include "declarations.hpp"

#include <platform.h>

#include <v8.h>

namespace interop::platform_v8 {

class object_t final: public internals::object_caching_layer_t<platform_object_t> {
    using base_t = internals::object_caching_layer_t<platform_object_t>;

    v8::UniquePersistent<v8::Object> handle;
    const module_t & module;
    const object_t * parent = nullptr;

  public:
    object_t(std::string name, v8::Local<v8::Object>, const module_t &);
    object_t(std::string name, v8::Local<v8::Object>, const object_t &);

    const module_t & get_module() const { return module; }
    v8::Local<v8::Object> get_handle() const;

  private:
    property_ptr_t fetch_property(const std::string_view & name) const;

    function_ptr_t fetch_function(const std::string_view & name) const override;
    field_ptr_t fetch_field(const std::string_view & name) const override;
};

} // namespace interop::platform_v8
