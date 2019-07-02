#include "object.hpp"
#include "common.hpp"
#include "function.h"
#include "module.h"

using namespace std;
using namespace v8;

namespace interop {
platform_v8_object_t::platform_v8_object_t(string name, Local<Object> handle,
                                           platform_v8_module_t & module)
  : platform_object_t(move(name))
  , handle(module.get_isolate(), handle)
  , module(module)
{}

function_ptr_t platform_v8_object_t::function(const std::string_view & name) const
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

    if (!property->IsFunction()) {
        throw error_t(object_name + "." + name.data() + " is not a function");
    }

    return make_shared<platform_v8_function_t>(name.data(), property.As<Function>(), *this);
}

Local<Object> platform_v8_object_t::get_handle() const { return handle.Get(module.get_isolate()); }

} // namespace interop
