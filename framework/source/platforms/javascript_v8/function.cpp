//
// Created by islam on 09.04.18.
//

#include "function.h"
#include "common.hpp"
#include "module.h"

using namespace std;
using namespace v8;

namespace interop {
namespace {
/*-----------<< utility >>------------*/
val_t from_v8(Isolate * isolate, const Local<Value> & value)
{
    if (value.IsEmpty()) {
        return {};
    }

    if (value->IsBoolean()) {
        return value->ToBoolean(isolate)->Value();
    } else if (value->IsInt32()) { // TODO: find a better solution
        return value->ToInt32(isolate)->Value();
    } else if (value->IsString()) {
        return string(*String::Utf8Value(isolate, value->ToString(isolate)));
    } else if (value->IsNumber()) {
        return value->ToNumber(isolate)->Value();
    }

    return {};
}

Local<Value> to_v8(const val_t & value, Isolate * isolate)
{
    const auto & type = value.type();

    if (type == typeid(bool)) {
        return Boolean::New(isolate, value.as<bool>());
    } else if (type == typeid(int)) {
        return Int32::New(isolate, value.as<int>());
    } else if (type == typeid(float)) {
        return Number::New(isolate, value.as<float>());
    } else if (type == typeid(double)) {
        return Number::New(isolate, value.as<double>());
    } else if (type == typeid(string)) {
        return helpers::to_v8(isolate, value.as<string>());
    } else {
        return Undefined(isolate);
    }
}

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

    const auto & ret_val = fn->ffi_call(std::move(args));
    if (!ret_val.empty()) {
        info.GetReturnValue().Set(to_v8(ret_val, isolate));
    }
}

/*----------->> utility <<------------*/
} // namespace

platform_function_v8_t::platform_function_v8_t(Handle<Function> && _handle,
                                               platform_v8_module_t & _module)
  : handle(_module.get_isolate(), _handle)
  , module(_module)
{}

val_t platform_function_v8_t::call(const arg_pack_t & args) const
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
        v8_args.push_back(to_v8(arg, isolate));
    }

    return from_v8(isolate, handle.Get(isolate)->Call(global, (int)v8_args.size(), v8_args.data()));
}

void platform_function_v8_t::expose_function_view(v8::Isolate * isolate,
                                                  v8::Local<v8::Object> object,
                                                  v8::Local<v8::Context> context,
                                                  const function_ptr_t & function)
{
    const auto & data = External::New(isolate, function.get());
    const auto & name = function->get_metadata().name;

    // FIXME: return value is probably useful
    std::ignore = object->Set(context, helpers::to_v8(isolate, name),
                              Function::New(isolate, exposed_function_callback, data));
}

} // namespace interop
