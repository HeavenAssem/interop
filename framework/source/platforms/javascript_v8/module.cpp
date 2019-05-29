//
// Created by islam on 09.04.18.
//

#include "module.h"
#include "common.hpp"
#include "function.h"
#include "object.hpp"

#include <configuration.h>
#include <logger.hpp>
#include <native_module.h>
#include <node.hpp>
#include <os.h>
#include <utils/string.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

using namespace std;
using namespace v8;

namespace interop {
namespace {

void constructor_callback(const FunctionCallbackInfo<Value> & args)
{
    auto isolate   = args.GetIsolate();
    auto & manager = *static_cast<object_manager_t *>(args.Data().As<External>()->Value());
    auto & module  = manager.get_module();
    auto & name    = manager.get_class_name();

    if (!args.IsConstructCall()) {
        isolate->ThrowException(helpers::to_v8_str(isolate, "Cannot call constructor as function"));
    }

    HandleScope scope(isolate);

    auto js_object  = args.This();
    auto object_ptr = module.create_dynamic(name, helpers::from_v8(args));

    js_object->SetAlignedPointerInInternalField(0, object_ptr.get());
    js_object->SetAlignedPointerInInternalField(1, &manager);

    manager.manage_lifetime(std::move(object_ptr), js_object);
}

void method_callback(const FunctionCallbackInfo<Value> & args)
{
    const auto & object =
        *static_cast<object_view_t *>(args.This()->GetAlignedPointerFromInternalField(0));
    const auto & method_metadata =
        *static_cast<function_metadata_t *>(args.Data().As<External>()->Value());
    auto isolate = args.GetIsolate();

    args.GetReturnValue().Set(helpers::to_v8(
        isolate, object.function(method_metadata.name)->call_dynamic(helpers::from_v8(args))));
}

} // namespace

platform_v8_module_t::platform_v8_module_t(Isolate * isolate,
                                           const platform_module_configuration_t & configuration)
  : internal_module_t(configuration.name)
  , isolate(isolate)
{
    // Enter the isolate
    v8::Isolate::Scope isolate_scope(isolate);
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(isolate);

    auto global_template = ObjectTemplate::New(isolate);

    // Create a new context.
    auto local_context = Context::New(isolate, nullptr, global_template);
    // Store context
    persistent_context = UniquePersistent<Context>(isolate, local_context);
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

void platform_v8_module_t::link(node_t & node)
{
    // Enter the isolate
    v8::Isolate::Scope isolate_scope(isolate);
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(isolate);

    auto context = get_context();
    // Enter the context
    Context::Scope context_scope(context);

    auto global = context->Global();

    // Expose all modules
    for (auto & module : node.iterate_modules()) {
        if (&module == this) {
            continue;
        }

        auto current_object = global;
        auto path           = utils::split_rx(module.get_name(), "\\.");

        interop_invariant_m(!path.empty(), "empty module name");

        auto name = move(path.back());
        path.resize(path.size() - 1);

        for (const auto & name : path) {
            auto key      = helpers::to_v8_str(isolate, name);
            auto existing = current_object->Get(context, key).ToLocalChecked();
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

        auto key = helpers::to_v8_str(isolate, name);

        if (current_object->Has(key)) {
            throw error_t("name collision during V8 module link: " + name + " already exists");
        }

        if (auto native_module_ptr = dynamic_cast<native_module_t *>(&module)) {
            expose_module(current_object, key, *native_module_ptr);
        } else {
            expose_module(current_object, key, module);
        }
    }

    // Initialize module
}

object_ptr_t platform_v8_module_t::create_dynamic(const std::string_view & name, arg_pack_t args)
{
    // Enter the isolate
    v8::Isolate::Scope isolate_scope(isolate);
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(isolate);

    auto context = get_context();
    // Enter the context
    Context::Scope context_scope(context);

    auto constructor = dynamic_pointer_cast<platform_v8_function_t>(function(name));

    return make_unique<platform_v8_object_t>(name.data(), constructor->constructor_call(move(args)),
                                             *this);
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

    auto context = get_context();
    // Enter the context
    Context::Scope context_scope(context);

    auto global = context->Global();
    auto value  = global->Get(helpers::to_v8_str(isolate, name));

    if (!value->IsFunction()) {
        throw function_lookup_error_t("function '"s + name.data() + "' was not found in module '" +
                                      get_name() +
                                      "'; found instead: " + helpers::to_string(isolate, value));
    }

    return make_shared<platform_v8_function_t>(name.data(), value.As<Function>(), *this);
}

void platform_v8_module_t::unload()
{
    base::unload();

    // Dispose context
    persistent_context.Reset();

    // Dispose the isolate
    if (isolate) {
        isolate->Dispose();
    }
}

void platform_v8_module_t::initiate_garbage_collection_for_testing() const
{
    isolate->LowMemoryNotification();
    // using namespace chrono_literals;
    // while (!isolate->IdleNotificationDeadline(0.5)) {
    //     this_thread::sleep_for(1s);
    // };
    // isolate->RequestGarbageCollectionForTesting(v8::Isolate::kFullGarbageCollection);
}

void platform_v8_module_t::initialize() const {}

void platform_v8_module_t::expose_module(v8::Local<v8::Object> parent, v8::Local<v8::String> key,
                                         native_module_t & module)
{
    auto context = get_context();

    auto root = v8::Object::New(isolate);

    for (const auto & function_metadata : module.iterate_functions()) {
        platform_v8_function_t::expose_function_view(isolate, root, context,
                                                     module.function(function_metadata.name));
    }

    for (const auto & [class_id, class_metadata] : module.iterate_classes()) {
        const auto & name = class_metadata.name;

        auto tpl = FunctionTemplate::New(
            isolate, helpers::forward_exceptions<constructor_callback>,
            External::New(isolate, &class_manager.get_manager_for_class(class_id)));

        tpl->SetClassName(helpers::to_v8_str(isolate, name));
        tpl->InstanceTemplate()->SetInternalFieldCount(2); // object itself, manager

        auto prototype = tpl->PrototypeTemplate();

        for (const auto & method_metadata : class_metadata.methods) {
            // Add object properties to the prototype
            // Methods, Properties, etc.
            prototype->Set(
                helpers::to_v8_str(isolate, method_metadata.name),
                FunctionTemplate::New(
                    isolate, helpers::forward_exceptions<method_callback>,
                    External::New(isolate, const_cast<function_metadata_t *>(&method_metadata))));
        }

        root->Set(helpers::to_v8_str(isolate, name), tpl->GetFunction());
    }

    parent->Set(key, root);
}

void platform_v8_module_t::expose_module(v8::Local<v8::Object> parent, v8::Local<v8::String> key,
                                         internal_module_t & module)
{
    ignore = parent;
    ignore = key;
    ignore = module;

    throw not_implemented("V8 JS to non-native module interoperability is not implemented");
}

} // namespace interop
