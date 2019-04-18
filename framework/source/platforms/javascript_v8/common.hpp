#pragma once

#include <declarations.h>
#include <exceptions.h>
#include <logger.hpp>

#include <v8.h>

namespace interop {
namespace helpers {
inline v8::Local<v8::String> to_v8_str(v8::Isolate * isolate, const std::string_view & in,
                                       v8::NewStringType type = v8::NewStringType::kNormal)
{
    auto v8_string = v8::String::NewFromUtf8(isolate, in.data(), type, in.size());
    if (v8_string.IsEmpty()) {
        throw error_t("v8 failed to allocate string of size " + std::to_string(in.size()));
    }
    return v8_string.ToLocalChecked();
}

inline std::string to_string(v8::Isolate * isolate, const v8::Local<v8::Value> & v8_value)
{
    return *v8::String::Utf8Value(isolate, v8_value);
}

inline val_t from_v8(v8::Isolate * isolate, const v8::Local<v8::Value> & value)
{
    if (value.IsEmpty() || value->IsNullOrUndefined()) {
        return {};
    }

    if (value->IsBoolean()) {
        return value.As<v8::Boolean>()->Value();
    } else if (value->IsInt32()) { // TODO: find a better solution
        return value.As<v8::Int32>()->Value();
    } else if (value->IsNumber()) {
        return value.As<v8::Number>()->Value();
    }

    return to_string(isolate, value);
}

inline val_t call_v8(v8::Local<v8::Context> context, v8::Local<v8::Value> recv,
                     v8::Local<v8::Function> func, std::vector<v8::Local<v8::Value>> args)
{
    v8::Isolate * isolate = context->GetIsolate();

    v8::TryCatch try_catch(isolate);

    v8::MaybeLocal<v8::Value> result = func->Call(context, recv, args.size(), args.data());

    if (try_catch.HasCaught()) {
        throw error_t("JS exception: " + to_string(isolate, try_catch.Exception()));
    }

    interop_invariant_m(!result.IsEmpty(), "no exception caught, but MaybeLocal is empty");

    return from_v8(isolate, result.ToLocalChecked());
}

inline val_t call_v8(v8::Local<v8::Context> context, v8::Local<v8::Function> func,
                     std::vector<v8::Local<v8::Value>> args)
{
    return call_v8(context, context->Global(), func, std::move(args));
}

inline v8::Local<v8::Object> call_v8_as_constructor(v8::Local<v8::Context> context,
                                                    v8::Local<v8::Function> func,
                                                    std::vector<v8::Local<v8::Value>> args)
{
    v8::Isolate * isolate = context->GetIsolate();

    v8::TryCatch try_catch(isolate);

    v8::MaybeLocal<v8::Object> result = func->NewInstance(context, args.size(), args.data());

    if (try_catch.HasCaught()) {
        throw std::runtime_error("JS exception: " + to_string(isolate, try_catch.Exception()));
    }

    interop_invariant_m(!result.IsEmpty(), "no exception caught, but MaybeLocal is empty");

    v8::Local<v8::Object> local = result.ToLocalChecked();

    interop_invariant_m(!local.IsEmpty(), "NewInstance returned empty Local");

    return local;
}

inline v8::Local<v8::Value> to_v8(v8::Isolate * isolate, const val_t & value)
{
    const auto & type = value.type();

    if (type == typeid(bool)) {
        return v8::Boolean::New(isolate, value.as<bool>());
    } else if (type == typeid(int)) {
        return v8::Int32::New(isolate, value.as<int>());
    } else if (type == typeid(float)) {
        return v8::Number::New(isolate, value.as<float>());
    } else if (type == typeid(double)) {
        return v8::Number::New(isolate, value.as<double>());
    } else if (type == typeid(std::string)) {
        return to_v8_str(isolate, value.as<std::string>());
    } else {
        return v8::Undefined(isolate);
    }
}

inline std::vector<v8::Local<v8::Value>> to_v8(v8::Isolate * isolate, arg_pack_t args)
{
    std::vector<v8::Local<v8::Value>> v8_args;
    v8_args.reserve(args.size());

    for (const auto & arg : args) {
        v8_args.push_back(helpers::to_v8(isolate, arg));
    }

    return v8_args;
}

inline arg_pack_t from_v8(const v8::FunctionCallbackInfo<v8::Value> & v8_args)
{
    arg_pack_t args;
    args.reserve(v8_args.Length());
    auto isolate = v8_args.GetIsolate();

    for (int i = 0; i < v8_args.Length(); ++i) {
        args.push_back(from_v8(isolate, v8_args[i]));
    }

    return args;
}

template <v8::FunctionCallback callback>
void forward_exceptions(const v8::FunctionCallbackInfo<v8::Value> & info)
{
    auto isolate = info.GetIsolate();
    try {
        callback(info);
    } catch (const std::exception & e) {
        info.GetReturnValue().Set(isolate->ThrowException(to_v8_str(isolate, e.what())));
    } catch (...) {
        info.GetReturnValue().Set(
            isolate->ThrowException(to_v8_str(isolate, "unknown C++ exception")));
    }
}

} // namespace helpers
} // namespace interop
