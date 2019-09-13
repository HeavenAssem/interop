#pragma once

#include <v8.h>

namespace interop::platform_v8 {

class property_t {
  public:
    virtual ~property_t() = default;

    virtual v8::Local<v8::Value> get_handle() const = 0;
};

} // namespace interop::platform_v8
