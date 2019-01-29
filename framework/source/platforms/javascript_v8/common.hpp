#pragma once

#include <exceptions.h>
#include <v8.h>

namespace interop {
namespace helpers {
v8::Local<v8::String> to_v8(v8::Isolate* isolate, const std::string& in, v8::NewStringType type = v8::NewStringType::kNormal) {
    auto v8_string = v8::String::NewFromUtf8(isolate, in.data(), type, in.size());
    if (v8_string.IsEmpty()) {
        throw error_t("v8 failed to allocate string of size " + std::to_string(in.size()));
    }
    return v8_string.ToLocalChecked();
}

}
}
