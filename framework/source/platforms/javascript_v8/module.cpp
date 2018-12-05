//
// Created by islam on 09.04.18.
//

#include "module.h"
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

    // global_template->Set(
    //     String::NewFromUtf8(isolate, "get"),
    //     FunctionTemplate::New(isolate, [](const FunctionCallbackInfo<Value> & info) {}));

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
            auto retVal =
                Script::Compile(local_context, String::NewFromUtf8(isolate, source_str.c_str(),
                                                                   NewStringType::kNormal)
                                                   .ToLocalChecked())
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

    global->Set(String::NewFromUtf8(isolate, "lol"), Object::New(isolate));

    node.for_each_module([&](const module_ptr & module) {
        if (module.get() == this) {
            return Continue;
        }

        const auto metadata = module->get_metadata();

        auto current_object = global;
        auto path           = utils::split_rx(metadata.name, "\\.");

        for (const auto & name : path) {
            auto new_object = Object::New(isolate);
            cout << "add object: " << name << endl;
            current_object->Set(
                String::NewFromUtf8(isolate, name.data(), String::kNormalString, name.size()),
                new_object);
            current_object = new_object;
        }

        for (const auto & function_metadata : metadata.functions) {
            const auto & name = function_metadata.name;

            std::ignore = name;  // FIXME: do something!

            // AccessorNameGetterCallback cb = [](Local<Name> property,
            //                                    const PropertyCallbackInfo<Value> & info) {

            // };

            // current_object->SetAccessor(
            //     local_context,
            //     String::NewFromUtf8(isolate, name.data(), String::kNormalString, name.size()),
            //     cb);
        }
        return Continue;
    });
}

void platform_v8_module_t::listen(const std::string_view & module_name,
                                  std::function<void()> && handler)
{
    std::ignore = module_name;
    std::ignore = handler;
    throw not_implemented("platform_v8_module_t::listen");
}

function_ptr_t platform_v8_module_t::fetch_function(const std::string & name)
{
    // Enter the isolate
    v8::Isolate::Scope isolate_scope(isolate);
    // Create a stack-allocated handle scope.
    HandleScope handle_scope(isolate);

    auto local_context = context.Get(isolate);
    // Enter the context
    Context::Scope context_scope(local_context);

    auto global = local_context->Global();
    auto value  = global->Get(String::NewFromUtf8(isolate, name.c_str()));

    if (value->IsFunction()) {
        function_metadata_t meta;

        meta.name = name;

        return make_shared<function_view_t>(
            make_shared<platform_function_v8_t>(Handle<Function>::Cast(value), *this), meta);
    }

    throw function_lookup_error_t("function '" + name + "' was not found in module '" +
                                  this->name() + "'");
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
