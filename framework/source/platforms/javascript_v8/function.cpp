//
// Created by islam on 09.04.18.
//

#include "function.h"
#include "module.h"

using namespace std;
using namespace v8;

namespace mosaic {
    namespace {
        /*-----------<< utility >>------------*/
        val_t from_v8(const Local<Value> & value) {
            if (value->IsInt32()) { // TODO: find a better solution
                return value->ToInt32()->Value();
            } else if (value->IsString()) {
                return string(*String::Utf8Value(value->ToString()));
            } else if (value->IsNumber()) {
                return value->ToNumber()->Value();
            }

            return {};
        }

        Local<Value> to_v8(const val_t & value, Isolate * isolate) {
            const auto & type = value.type();

            if (type == typeid(int)) {
                return Int32::New(isolate, any_cast<int>(value));
            } else if (type == typeid(float)) {
                return Number::New(isolate, any_cast<float>(value));
            } else if (type == typeid(double)) {
                return Number::New(isolate, any_cast<double>(value));
            } else if (type == typeid(string)) {
                return String::NewFromUtf8(isolate, any_cast<string>(value).c_str(), NewStringType::kNormal).ToLocalChecked();
            } else {
                return Undefined(isolate);
            }
        }
        /*----------->> utility <<------------*/
    }

    platform_function_v8_t::platform_function_v8_t(Handle<Function> && _handle, platform_v8_module_t & _module)
        : handle(_module.get_isolate(), _handle)
        , module(_module)
    {}

    val_t platform_function_v8_t::call(arg_pack_t && args)
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

        for (const auto & arg: args) {
            v8_args.push_back(to_v8(arg, isolate));
        }

        return from_v8(handle.Get(isolate)->Call(global, (int)v8_args.size(), v8_args.data()));
    }
}
