//
// Created by islam on 07.05.17.
//

#include "node.h"
#include "native_module.h"
#include "platform_factory.h"
#include "configuration.h"

#include <os.h>
#include <platform.h>

#include <exceptions.h>
#include <logger.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

using namespace std;
namespace fs = boost::filesystem;

namespace mosaic {

    //std::unordered_map<std::string, std::shared_ptr<native_module_t>> node_t::global_scope {};

    node_t::node_t(const node_configuration_t & configuration) {
        name = configuration.name;

        for (const auto & native_module_configuration: configuration.native_module_configurations) {
            try {
                load_native_module(native_module_configuration);
            } catch (error_t & e) {
                mosaic_logger(error, e.what());
            }
        }

        for (const auto & id_platform_configuration: configuration.platform_configurations) {
            const auto & [id, platform_configuration] = id_platform_configuration;

            auto & platform = platforms[id];
            platform = instantiate_platform(id);
            for (auto & module: platform->initialize(platform_configuration)) {
                local_scope[module->name()] = move(module);
            }
        }

        // for (auto filename : os::walk(directory, os::library_extension, os::Recursive, os::File)) {

        // }

        mosaic_logger(log, "created node '" + name + "'");
    }

    node_t::node_t(node_t && other)
    {}


    module_view & node_t::get(const std::string & name) {
        try {
            return *local_scope.at(name);
        } catch (const out_of_range &) {
            throw module_lookup_error("module with name '" + name + "' was not registered");
        }
    }

    node_t::~node_t() {
        forced_unload();
    }

    void node_t::unload(bool forced) {
        if (forced) {
            forced_unload();
        }
        else {
            for (auto it = local_scope.begin(); it != local_scope.end(); it = local_scope.erase(it)) {
                auto & module_name      = it->first;
                auto & module_pointer   = it->second;

                module_pointer->unload();
                //global_scope.erase(module_name);
            }
        }
    }

    void node_t::forced_unload() noexcept {
        for (auto it = local_scope.begin(); it != local_scope.end(); it = local_scope.erase(it)) {
            auto & module_name      = it->first;
            auto & module_pointer   = it->second;

            try {
                module_pointer->unload();
                //global_scope.erase(module_name);
            } catch (error_t & e) {
                mosaic_logger(log, string("error during forced unload: ") + e.what());
            }
        }
    }

    void node_t::link() {
        for (auto & kv : local_scope) {
            kv.second->link(*this);
        }
    }

    void node_t::load_native_module(const native_module_configuration_t & configuration) {
        auto library_instance = shared_library(configuration.path.c_str());
        auto module = make_unique<native_module_t>(move(library_instance), configuration);

        mosaic_logger(log, "registered native module '" + module->name() + "'");

        local_scope[module->name()] = move(module);
        //global_scope[module->name()] = module;
    }
}
