//
// Created by islam on 07.11.17.
//

#include "platform.h"
#include "platform_factory.h"
#include "module.h"
#include "configuration.h"

#include <logger.h>

#include <v8.h>
#include <libplatform/libplatform.h>

#include <iostream>

using namespace std;
using namespace v8;


namespace mosaic {

    class platform_v8_t final: public platform_t {
        Platform               * platform  = nullptr;
        ArrayBuffer::Allocator * allocator = nullptr;

    public:
        /*-----------<< public interface implementation >>------------*/
        vector<unique_ptr<base_module_t>> initialize(const platform_configuration_t & configuration) override {
            V8::InitializeICUDefaultLocation(configuration.execution_path.c_str());
            V8::InitializeExternalStartupData(configuration.execution_path.c_str());
            platform  = v8::platform::CreateDefaultPlatform();
            allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
            V8::InitializePlatform(platform);
            V8::Initialize();

            // Create a new Isolate and make it the current one.
            Isolate::CreateParams create_params;
            create_params.array_buffer_allocator = allocator;

            unordered_set<string> names;
            vector<unique_ptr<base_module_t>> modules;
            modules.reserve(configuration.module_configurations.size());

            for (const auto & module_configuration: configuration.module_configurations) {
                try {
                    if (!names.insert(module_configuration.name).second) {
                        throw module_loading_error("Module with name '" + module_configuration.name + "' already exists");
                    } else {
                        modules.push_back(make_unique<platform_v8_module_t>(Isolate::New(create_params), module_configuration));
                    }
                } catch (const module_loading_error & e) {
                    mosaic_logger(error, e.what());
                }
            }

            return modules;
        }

        void run() override {
            // Isolate::Scope isolate_scope(isolate);
            // // Create a stack-allocated handle scope.
            // HandleScope handle_scope(isolate);

            // auto local_context = context.Get(isolate);
            // // Enter the context for compiling and running the hello world script.
            // Context::Scope context_scope(local_context);
            // {
            //     // Create a string containing the JavaScript source code.
            //     Local<String> source = String::NewFromUtf8(isolate, "'Hello' + ', World!'",
            //                                                NewStringType::kNormal).ToLocalChecked();
            //     // Compile the source code.
            //     Local<Script> script = Script::Compile(local_context, source).ToLocalChecked();
            //     // Run the script to get the result.
            //     Local<Value> result = script->Run(local_context).ToLocalChecked();
            //     // Convert the result to an UTF8 string and print it.
            //     String::Utf8Value utf8(result);
            //     printf("%s\n", *utf8);
            // }

            // {
            //     auto src = " function test() {return 42;} ";

            //     Local<String> source =
            //         String::NewFromUtf8(isolate, src,
            //                             NewStringType::kNormal).ToLocalChecked();
            //     // Compile the source code.
            //     Local<Script> script = Script::Compile(local_context, source).ToLocalChecked();
            //     // Run the script to get the result.
            //     script->Run(local_context);
            // }
        }

        void dispose() override {
            // Dispose the isolate and tear down V8.
            if (platform) {
                V8::Dispose();
                V8::ShutdownPlatform();
                delete platform;
                delete allocator;

                platform  = nullptr;
                allocator = nullptr;
            }
        }
        /*----------->> public interface implementation <<------------*/

    };

    register_platform_as("js", platform_v8_t)
}
