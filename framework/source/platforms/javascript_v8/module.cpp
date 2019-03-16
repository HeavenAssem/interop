//
// Created by islam on 09.04.18.
//

#include "module.h"
#include "common.hpp"
#include "configuration.h"
#include "function.h"
#include "node.h"

#include <logger.h>
#include <os.h>
#include <utils/string.h>

#include <fstream>
#include <sstream>

#include <iostream>

using namespace std;
using namespace v8;

namespace interop {

platform_v8_module_t::platform_v8_module_t(Isolate * isolate,
                                           const platform_module_configuration_t & configuration)
  : base_module_t()
  , isolate(isolate)
{
    metadata.name = configuration.name;

    // Enter the isolate
    v8::Isolate::Scope isolate_scope(isolate);
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(isolate);

    auto global_template = ObjectTemplate::New(isolate);

    // Create a new context.
    auto local_context = Context::New(isolate, nullptr, global_template);
    // Store context
    context = UniquePersistent<Context>(isolate, local_context);
    // Enter the context for compiling and running scripts.
    Context::Scope context_scope(local_context);

    auto runScript = [&](const string & filename) {
        ifstream source(filename);
        if (source.is_open()) {
            stringstream buffer;
            buffer << source.rdbuf();
            const auto & source_str = buffer.str();
            auto retVal = Script::Compile(local_context, helpers::to_v8_str(isolate, source_str))
                              .ToLocalChecked()
                              ->Run(local_context)
                              .ToLocalChecked();

            interop_logger(log, "Run JS script " + filename + ": " +
                                    (*String::Utf8Value(isolate, retVal)));
            return true;
        } else {
            interop_logger(error, "Run JS script " + filename + " failed: unable to open file");
            return false;
        }
    };

    bool ok = false;

    for (const auto & path : configuration.paths) {
        if (os::get_entry_type(path.c_str()) == os::Directory) {
            for (const auto & filename : os::walk(path.c_str(), ".js", os::Recursive, os::File)) {
                ok |= runScript(filename);
            }
        } else {
            ok |= runScript(path.c_str());
        }
    }

    if (!ok) {
        throw module_loading_error_t(
            "Unable to load JS module: couldn't open any of specified files");
    }
}

platform_v8_module_t::~platform_v8_module_t() {}

void platform_v8_module_t::link(node_t & node) const
{
    // Enter the isolate
    v8::Isolate::Scope isolate_scope(isolate);
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(isolate);

    auto local_context = context.Get(isolate);
    // Enter the context
    Context::Scope context_scope(local_context);

    auto global = local_context->Global();

    node.for_each_module([&](const module_ptr & module) {
        if (module.get() == this) {
            return Continue;
        }

        const auto & metadata = module->get_metadata();

        auto current_object = global;
        auto path           = utils::split_rx(metadata.name, "\\.");

        for (const auto & name : path) {
            auto key      = helpers::to_v8_str(isolate, name);
            auto existing = current_object->Get(local_context, key).ToLocalChecked();
            if (existing->IsUndefined()) {
                auto new_object = Object::New(isolate);
                current_object->Set(key, new_object);
                current_object = new_object;
            } else if (!existing->IsObject()) {
                throw error_t("name collision during V8 module link: " + name +
                              " already exists and it's not an object");
            } else {
                current_object = existing.As<Object>();
            }
        }

        for (const auto & function_metadata : metadata.functions) {
            platform_function_v8_t::expose_function_view(isolate, current_object, local_context,
                                                         module->function(function_metadata.name));
        }

        for (const auto & type_metadata : metadata.types) {
            const auto & name = type_metadata.name;

            auto tpl = FunctionTemplate::New(
                isolate,
                [](const FunctionCallbackInfo<Value> & args) {
                    auto isolate = args.GetIsolate();
                    const auto & module =
                        *static_cast<const base_module_t *>(args.Data().As<External>()->Value());

                    if (!args.IsConstructCall()) {
                        isolate->ThrowException(
                            helpers::to_v8_str(isolate, "Cannot call constructor as function"));
                    }

                    HandleScope scope(isolate);
                    auto js_object  = args.This();
                    auto js_pobject = Persistent<Object>(isolate, js_object);

                    // TODO: avoid conversion: get from another place
                    const auto & name = helpers::from_v8(isolate, js_object->GetConstructorName());

                    auto object = module.create_dynamic(name, helpers::from_v8(args));
                    js_object->SetAlignedPointerInInternalField(0, object.get());
                    js_pobject.SetWeak(object.release(),
                                       [](const WeakCallbackInfo<object_view_t> & data) {
                                           delete data.GetParameter();
                                       },
                                       WeakCallbackType::kParameter);
                },
                External::New(isolate, module.get()));

            tpl->SetClassName(helpers::to_v8_str(isolate, name));
            tpl->InstanceTemplate()->SetInternalFieldCount(2); // module_view, metadata

            auto prototype = tpl->PrototypeTemplate();

            for (const auto & method_metadata : type_metadata.methods) {
                // Add object properties to the prototype
                // Methods, Properties, etc.
                prototype->Set(
                    helpers::to_v8_str(isolate, method_metadata.name),
                    FunctionTemplate::New(
                        isolate,
                        [](const FunctionCallbackInfo<Value> & args) {
                            const auto & object = *static_cast<object_view_t *>(
                                args.This()->GetAlignedPointerFromInternalField(0));
                            const auto & method_metadata = *static_cast<function_metadata_t *>(
                                args.Data().As<External>()->Value());
                            auto isolate = args.GetIsolate();

                            args.GetReturnValue().Set(helpers::to_v8(
                                isolate, object.function(method_metadata.name)
                                             ->dynamic_call(helpers::from_v8(args))));
                        },
                        External::New(isolate,
                                      const_cast<function_metadata_t *>(&method_metadata))));
            }

            current_object->Set(helpers::to_v8_str(isolate, name), tpl->GetFunction());
        }

        return Continue;
    });
}

object_ptr_t platform_v8_module_t::create_dynamic(const std::string_view & name,
                                                  arg_pack_t args) const
{
    std::ignore = name;
    std::ignore = args;
    throw not_implemented("platform_v8_module_t::create_dynamic");
}

void platform_v8_module_t::listen(const std::string_view & module_name,
                                  std::function<void()> && handler)
{
    std::ignore = module_name;
    std::ignore = handler;
    throw not_implemented("platform_v8_module_t::listen");
}

function_ptr_t platform_v8_module_t::fetch_function(const std::string_view & name)
{
    // Enter the isolate
    v8::Isolate::Scope isolate_scope(isolate);
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(isolate);

    auto local_context = context.Get(isolate);
    // Enter the context
    Context::Scope context_scope(local_context);

    auto global = local_context->Global();
    auto value  = global->Get(helpers::to_v8_str(isolate, name));

    if (!value->IsFunction()) {
        throw function_lookup_error_t("function '"s + name.data() + "' was not found in module '" +
                                      this->name() + "'");
    }

    return make_shared<platform_function_v8_t>(name.data(), Handle<Function>::Cast(value), *this);
}

const string & platform_v8_module_t::name() const { return metadata.name; }

void platform_v8_module_t::unload()
{
    base::unload();

    // Dispose context
    context.Reset();

    // Dispose the isolate
    if (isolate) {
        isolate->Dispose();
    }
}

Isolate * platform_v8_module_t::get_isolate() { return isolate; }

UniquePersistent<Context> & platform_v8_module_t::get_context() { return context; }
} // namespace interop
