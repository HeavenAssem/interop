#pragma once

#include <declarations.h>
#include <exceptions.h>

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

inline std::string from_v8(v8::Isolate * isolate, const v8::Local<v8::String> & v8_str)
{
    return *v8::String::Utf8Value(isolate, v8_str);
}

inline val_t from_v8(v8::Isolate * isolate, const v8::Local<v8::Value> & value)
{
    if (value.IsEmpty()) {
        return {};
    }

    if (value->IsBoolean()) {
        return value->ToBoolean(isolate)->Value();
    } else if (value->IsInt32()) { // TODO: find a better solution
        return value->ToInt32(isolate)->Value();
    } else if (value->IsString()) {
        return from_v8(isolate, value->ToString(isolate));
    } else if (value->IsNumber()) {
        return value->ToNumber(isolate)->Value();
    }

    return {};
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

} // namespace helpers
} // namespace interop
