//
// Created by islam on 10.04.18.
//

#pragma once

#include "declarations.hpp"

#include <platform.h>

#include <v8.h>

namespace interop {
class platform_v8_function_t final: public platform_function_t {
    v8::UniquePersistent<v8::Function> handle;
    const platform_v8_module_t & module;
    const platform_v8_object_t * object = nullptr;

  public:
    platform_v8_function_t(std::string name, v8::Local<v8::Function>, const platform_v8_module_t &);
    platform_v8_function_t(std::string name, v8::Local<v8::Function>, const platform_v8_object_t &);

    val_t call_dynamic(arg_pack_t args) const override;
    v8::Local<v8::Object> constructor_call(arg_pack_t args) const;

    static void expose_function_view(v8::Isolate *, v8::Local<v8::Object>, v8::Local<v8::Context>,
                                     const function_ptr_t &);
};

} // namespace interop
