#include "object.hpp"
#include "common.hpp"
#include "field.hpp"
#include "function.h"
#include "module.h"

using namespace std;
using namespace v8;

namespace interop::platform_v8 {
object_t::object_t(string name, Local<Object> handle, const module_t & module)
  : base_t(move(name))
  , handle(helpers::current_isolate(), handle)
  , module(module)
{}

object_t::object_t(string name, Local<Object> handle, const object_t & parent)
  : base_t(move(name))
  , handle(helpers::current_isolate(), handle)
  , module(parent.get_module())
  , parent(&parent)
{}

property_ptr_t object_t::fetch_property(const std::string_view & name) const
{
    auto isolate = module.get_isolate();

    // Enter the isolate
    Isolate::Scope isolate_scope(isolate);
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(isolate);

    auto local_context = module.get_context();

    // Enter the context
    Context::Scope context_scope(local_context);

    auto object = handle.Get(isolate);

    auto property = object->Get(local_context, helpers::to_v8_str(name)).ToLocalChecked();

    if (property->IsFunction()) {
        return make_shared<function_t>(name.data(), property.As<Function>(), *this);
    } else if (property->IsObject()) {
        return make_shared<object_t>(name.data(), property.As<Object>(), *this);
    } else {
        return make_shared<field_t>(name.data(), property, *this);
    }
    throw error_t(object_name + "." + name.data() + " is not a function");
}

function_ptr_t object_t::fetch_function(const std::string_view & name) const
{
    auto property_ptr = fetch_property(name);
    if (auto function_ptr = std::dynamic_pointer_cast<function_view_t>(property_ptr)) {
        return function_ptr;
    }

    throw function_lookup_error_t("function '"s + name.data() + "' was not found in object '" +
                                  this->name());
}

field_ptr_t object_t::fetch_field(const std::string_view & name) const
{
    auto property_ptr = fetch_property(name);
    if (auto field_ptr = std::dynamic_pointer_cast<field_view_t>(property_ptr)) {
        return field_ptr;
    }

    throw function_lookup_error_t("field '"s + name.data() + "' was not found in object '" +
                                  this->name());
}

Local<Object> object_t::get_handle() const { return handle.Get(module.get_isolate()); }

} // namespace interop::platform_v8
