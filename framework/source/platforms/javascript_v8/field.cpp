#include "field.hpp"
#include "common.hpp"
#include "object.hpp"

namespace interop::platform_v8 {

field_t::field_t(std::string name, v8::Local<v8::Value> handle, const module_t & module)
  : platform_field_t(std::move(name))
  , handle(helpers::current_isolate(), handle)
  , module(module)
{}

field_t::field_t(std::string name, v8::Local<v8::Value> handle, const object_t & object)
  : platform_field_t(std::move(name))
  , handle(helpers::current_isolate(), handle)
  , module(object.get_module())
  , object(&object)
{}

val_t field_t::get_dynamic_non_native() const
{
    return helpers::from_v8(handle.Get(helpers::current_isolate()));
}

void field_t::set_dynamic_non_native(val_t value)
{
    auto local = handle.Get(helpers::current_isolate());
    local      = helpers::to_v8(value);

    throw not_implemented("oh no! what to do now!?");
}

} // namespace interop::platform_v8
