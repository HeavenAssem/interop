//
// Created by islam on 10.04.18.
//

#pragma once

#include "declarations.hpp"

#include <platform.h>

#include <v8.h>

namespace interop::platform_v8 {

class function_t final: public platform_function_t {
    v8::UniquePersistent<v8::Function> handle;
    const module_t & module;
    const object_t * object = nullptr;

  public:
    function_t(std::string name, v8::Local<v8::Function>, const module_t &);
    function_t(std::string name, v8::Local<v8::Function>, const object_t &);

    val_t call_dynamic(arg_pack_t args) const override;
    v8::Local<v8::Object> constructor_call(arg_pack_t args) const;

    static v8::Local<v8::Function> to_v8(const function_ptr_t &);
};

} // namespace interop::platform_v8
