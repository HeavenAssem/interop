//
// Created by islam on 09.04.18.
//

#include "function.h"
#include "common.hpp"
#include "module.h"

using namespace std;
using namespace v8;
using namespace interop::helpers;

namespace interop {
namespace {
/*-----------<< utility >>------------*/

void exposed_function_callback(const FunctionCallbackInfo<Value> & info)
{
    auto isolate = info.GetIsolate();
    HandleScope scope(isolate);
    // FIXME: no lifetime guarantees whatsoever - options:
    // 1) create object whose lifetime is same as v8 function that will check weak function pointer
    //    and if needed will refetch it.
    // 2) same as 1 but refetching will be done by framework
    // 3) make function views persistent and update them instead of recreation
    // Analysis:
    // 1 - some overhead with unpredictable execution time in case of refetch: forget about RT case
    //     function fetch must be thread-safe
    // 2 - needs sync
    // 3 - needs sync, but simpler, probably best current solution
    auto fn = static_cast<function_view_t *>(info.Data().As<External>()->Value());

    arg_pack_t args;
    args.reserve(info.Length());

    for (int i = 0; i < info.Length(); ++i) {
        args.push_back(from_v8(isolate, info[i]));
    }

    const auto & ret_val = fn->dynamic_call(std::move(args));
    if (!ret_val.empty()) {
        info.GetReturnValue().Set(to_v8(isolate, ret_val));
    }
}

/*----------->> utility <<------------*/
} // namespace

platform_function_v8_t::platform_function_v8_t(std::string name, Handle<Function> && handle,
                                               platform_v8_module_t & module)
  : platform_function_t(std::move(name))
  , handle(module.get_isolate(), handle)
  , module(module)
{}

val_t platform_function_v8_t::dynamic_call(arg_pack_t args) const
{
    auto isolate = module.get_isolate();

    // Enter the isolate
    Isolate::Scope isolate_scope(isolate);
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(isolate);

    auto local_context = module.get_context().Get(isolate);

    // Enter the context
    Context::Scope context_scope(local_context);

    auto global = local_context->Global();

    vector<Local<Value>> v8_args;
    v8_args.reserve(args.size());

    for (const auto & arg : args) {
        v8_args.push_back(helpers::to_v8(isolate, arg));
    }

    return from_v8(isolate, handle.Get(isolate)->Call(global, (int)v8_args.size(), v8_args.data()));
}

void platform_function_v8_t::expose_function_view(v8::Isolate * isolate,
                                                  v8::Local<v8::Object> object,
                                                  v8::Local<v8::Context> context,
                                                  const function_ptr_t & function)
{
    // FIXME: return value is probably useful
    std::ignore = object->Set(
        context, to_v8(isolate, function->name),
        Function::New(isolate, exposed_function_callback, External::New(isolate, function.get())));
}

} // namespace interop
