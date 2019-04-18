#pragma once

#include "declarations.hpp"

#include <platform.h>

#include <v8.h>

namespace interop {
void expose_object_view();

class platform_v8_object_t final: public platform_object_t {
    v8::UniquePersistent<v8::Object> handle;
    platform_v8_module_t & module;

  public:
    platform_v8_object_t(std::string name, v8::Local<v8::Object>, platform_v8_module_t &);
    function_ptr_t function(const std::string_view & name) const override;

    platform_v8_module_t & get_module() const { return module; }
    v8::Local<v8::Object> get_handle() const;
};

} // namespace interop
