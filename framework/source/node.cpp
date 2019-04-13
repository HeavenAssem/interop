//
// Created by islam on 07.05.17.
//

#include "node.hpp"
#include "configuration.h"
#include "native_module.h"
#include "platform_factory.h"

#include <os.h>
#include <platform.h>

#include <exceptions.h>
#include <logger.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/scope_exit.hpp>

using namespace std;
namespace fs = boost::filesystem;

namespace interop {
namespace {
unique_ptr<node_t> node_instance_ptr;
}

node_t::node_t(const node_configuration_t & configuration)
{
    name = configuration.name;

    for (const auto & native_module_configuration : configuration.native_module_configurations) {
        try {
            load_native_module(native_module_configuration);
        } catch (error_t & e) {
            interop_logger(error, e.what());
            throw;
        }
    }

    for (const auto & [id, platform_configuration] : configuration.platform_configurations) {
        auto platform = instantiate_platform(id);
        for (auto & module_ptr : platform->initialize(platform_configuration)) {
            module_ptr->assign_id(modules.size());

            modules_by_name[module_ptr->name()] = module_ptr.get();
            modules.push_back(std::move(module_ptr));
        }

        platforms_by_name[id] = platform.get();
        platforms.push_back(std::move(platform));
    }

    // for (auto filename : os::walk(directory, os::library_extension, os::Recursive, os::File)) {

    // }

    interop_logger(log, "created node '" + name + "'");
}

node_t & node_t::start(const node_configuration_t & configuration)
{
    if (node_instance_ptr) {
        throw error_t("node hes already started");
    }

    node_instance_ptr.reset(new node_t(configuration));

    return *node_instance_ptr;
}

node_t & node_t::get()
{
    if (!node_instance_ptr) {
        throw error_t("no node instance");
    }

    return *node_instance_ptr;
}

module_view_t & node_t::get(const std::string & name)
{
    try {
        return *modules_by_name.at(name);
    } catch (const out_of_range &) {
        throw module_lookup_error_t("module with name '" + name + "' was not registered");
    }
}

node_t::~node_t() { forced_unload(); }

void node_t::unload(bool forced)
{
    if (forced) {
        forced_unload();
    } else {
        for (auto it = modules_by_name.begin(); it != modules_by_name.end();
             it      = modules_by_name.erase(it)) {
            auto & module_name    = it->first;
            auto & module_pointer = it->second;

            module_pointer->unload();
            std::ignore = module_name; // FIXME: do something!
            // global_scope.erase(module_name);
        }
    }
}

void node_t::forced_unload() noexcept
{
    for (auto it = modules_by_name.begin(); it != modules_by_name.end();
         it      = modules_by_name.erase(it)) {
        auto & module_name    = it->first;
        auto & module_pointer = it->second;

        try {
            module_pointer->unload();
            std::ignore = module_name; // FIXME: do something!
            // global_scope.erase(module_name);
        } catch (error_t & e) {
            interop_logger(log, string("error during forced unload: ") + e.what());
        }
    }
}

void node_t::link()
{
    for (auto & [_, module] : modules_by_name) {
        module->link(*this);
    }
}

node_t::modules_sequence_t node_t::iterate_modules()
{
    return [this, module_id = module_id_t{}]() mutable -> modules_sequence_t::value_t {
        BOOST_SCOPE_EXIT(&module_id) { ++module_id; }
        BOOST_SCOPE_EXIT_END

        for (; module_id < modules.size(); ++module_id) {
            if (const auto & module_ptr = modules[module_id]) {
                return {{module_id, *module_ptr}};
            }
        }

        return {};
    };
}

base_module_t & node_t::get(module_id_t id)
{
    interop_invariant_m(id < modules.size(), "invalid module id" << id);
    return *modules[id];
}

void node_t::load_native_module(const native_module_configuration_t & configuration)
{
    auto library_instance = shared_library(configuration.path.c_str());
    auto module_ptr = make_unique<native_module_t>(std::move(library_instance), configuration);

    module_ptr->assign_id(modules.size());

    interop_logger(log, "registered native module '" + module_ptr->name() + "'");

    modules_by_name[module_ptr->name()] = module_ptr.get();
    modules.push_back(std::move(module_ptr));
}
} // namespace interop
