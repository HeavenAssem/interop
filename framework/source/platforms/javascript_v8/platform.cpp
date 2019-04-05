//
// Created by islam on 07.11.17.
//

#include "platform.h"
#include "configuration.h"
#include "module.h"
#include "platform_factory.h"

#include <logger.hpp>

#include <libplatform/libplatform.h>
#include <v8.h>

#include <iostream>

using namespace std;
using namespace v8;

namespace interop {

class platform_v8_t final: public platform_t {
    Platform * platform                = nullptr;
    ArrayBuffer::Allocator * allocator = nullptr;

  public:
    /*-----------<< public interface implementation >>------------*/
    vector<unique_ptr<base_module_t>>
    initialize(const platform_configuration_t & configuration) override
    {
        // const char * argv[] = {"--expose_gc"};
        // int argc            = 1;

        V8::InitializeICUDefaultLocation(configuration.execution_path.c_str());
        V8::InitializeExternalStartupData(configuration.execution_path.c_str());
        platform  = v8::platform::CreateDefaultPlatform();
        allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
        V8::InitializePlatform(platform);
        V8::Initialize();
        // V8::SetFlagsFromCommandLine(&argc, const_cast<char **>(argv), false);

        // Create a new Isolate and make it the current one.
        Isolate::CreateParams create_params;
        create_params.array_buffer_allocator = allocator;

        unordered_set<string> names;
        vector<unique_ptr<base_module_t>> modules;
        modules.reserve(configuration.module_configurations.size());

        for (const auto & module_configuration : configuration.module_configurations) {
            try {
                if (!names.insert(module_configuration.name).second) {
                    throw module_loading_error_t("Module with name '" + module_configuration.name +
                                                 "' already exists");
                } else {
                    modules.push_back(make_unique<platform_v8_module_t>(Isolate::New(create_params),
                                                                        module_configuration));
                }
            } catch (const module_loading_error_t & e) {
                interop_logger(error, e.what());
            }
        }

        return modules;
    }

    void run() override {}
    void dispose() override
    {
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
} // namespace interop
