#pragma once

#include "declarations.hpp"
#include "property.hpp"

#include <platform.h>

#include <v8.h>

namespace interop::platform_v8 {

class field_t final: public platform_field_t, public property_t {
    v8::UniquePersistent<v8::Value> handle;
    const module_t & module;
    const object_t * object = nullptr;

  public:
    field_t(std::string name, v8::Local<v8::Value>, const module_t &);
    field_t(std::string name, v8::Local<v8::Value>, const object_t &);

    inline v8::Local<v8::Value> get_handle() const override
    {
        return handle.Get(v8::Isolate::GetCurrent());
    }

  private:
    val_t get_dynamic_non_native() const override;
    void set_dynamic_non_native(val_t) override;
};

} // namespace interop::platform_v8
