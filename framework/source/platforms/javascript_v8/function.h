//
// Created by islam on 10.04.18.
//

#pragma once

#include "platform.h"

#include <v8.h>

namespace interop {
class platform_v8_module_t;

class platform_function_v8_t final: public platform_function_t {
    v8::UniquePersistent<v8::Function> handle;
    platform_v8_module_t & module;

  public:
    platform_function_v8_t(v8::Handle<v8::Function> && handle, platform_v8_module_t & platform);

    val_t call(const arg_pack_t & args) const override;
};
} // namespace interop
