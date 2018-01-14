//
// Created by islam on 07.11.17.
//

#include "platform.h"
#include "platform_factory.h"

#include <v8.h>
#include <libplatform/libplatform.h>
#include <iostream>

using namespace std;
using namespace v8;


namespace mosaic {
    class platform_v8 final: public platform {
        Platform               * platform  = nullptr;
        Isolate                * isolate   = nullptr;
        ArrayBuffer::Allocator * allocator = nullptr;
        Eternal<Context>         context;
    public:
        void initialize(const string & execution_path) override {
            V8::InitializeICUDefaultLocation(execution_path.c_str());
            V8::InitializeExternalStartupData(execution_path.c_str());
            platform  = v8::platform::CreateDefaultPlatform();
            allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
            V8::InitializePlatform(platform);
            V8::Initialize();

            // Create a new Isolate and make it the current one.
            Isolate::CreateParams create_params;
            create_params.array_buffer_allocator = allocator;
            isolate = Isolate::New(create_params);
            {
                Isolate::Scope isolate_scope(isolate);
                // Create a stack-allocated handle scope.
                HandleScope handle_scope(isolate);
                // Create a new context.
                context = Eternal<Context>(isolate, Context::New(isolate));
            }
        }

        void run() override {
            Isolate::Scope isolate_scope(isolate);
            // Create a stack-allocated handle scope.
            HandleScope handle_scope(isolate);

            auto local_context = context.Get(isolate);
            // Enter the context for compiling and running the hello world script.
            Context::Scope context_scope(local_context);
            {
                // Create a string containing the JavaScript source code.
                Local<String> source = String::NewFromUtf8(isolate, "'Hello' + ', World!'",
                                                           NewStringType::kNormal).ToLocalChecked();
                // Compile the source code.
                Local<Script> script = Script::Compile(local_context, source).ToLocalChecked();
                // Run the script to get the result.
                Local<Value> result = script->Run(local_context).ToLocalChecked();
                // Convert the result to an UTF8 string and print it.
                String::Utf8Value utf8(result);
                printf("%s\n", *utf8);
            }

            {
                auto src = " function test() {return 42;} ";

                Local<String> source =
                    String::NewFromUtf8(isolate, src,
                                        NewStringType::kNormal).ToLocalChecked();
                // Compile the source code.
                Local<Script> script = Script::Compile(local_context, source).ToLocalChecked();
                // Run the script to get the result.
                script->Run(local_context);

                auto global = context.Get(isolate)->Global();
                auto value = global->Get(String::NewFromUtf8(isolate, "test"));

                if (value->IsFunction()) {
                    Handle<Value> args[0];
                    auto fn = Handle<Function>::Cast(value);
                    auto result = fn->Call(global, 0, args)->ToInt32()->Value();

                    cout << "js value: " << result << endl;
                }
            }
        }

        void dispose() override {
            // Dispose the isolate and tear down V8.
            if (isolate) {
                isolate->Dispose();
                V8::Dispose();
                V8::ShutdownPlatform();
                delete platform;
                delete allocator;

                platform  = nullptr;
                isolate   = nullptr;
                allocator = nullptr;
            }
        }
    };

    register_platform_as("js", platform_v8)
}
