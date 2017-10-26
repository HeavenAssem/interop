//
// Created by islam on 07.05.17.
//

#include "node.h"
#include "module.h"

#include <os.h>
#include <platform.h>

#include <exceptions.h>
#include <logger.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

using namespace std;
namespace fs = boost::filesystem;

namespace mosaic {

    std::unordered_map<std::string, std::shared_ptr<module>> node::global_scope {};

    node::node(const std::string_view & directory) {
        for (auto filename : os::walk(directory, library_extension)) {
            try {
                load_native_module(filename);
            } catch (error & e) {
                mosaic_logger(error, e.what());
            }
        }

        for (auto inner_directory : os::walk(directory, "", false, true)) {
            try {
                auto node_pointer = make_unique<node>(inner_directory);

                boost::filesystem::path path(inner_directory);
                nodes[fs::absolute(path).filename().string()] = move(node_pointer);
            } catch (node_loading_error & e) {          // it's fine if node creation fails inside another node,
                                                        // but if this exception leaves this class,
                                                        // then root node is empty which means that application is empty
            } catch (error & e) {
                mosaic_logger(error, e.what());
            }
        }

        if (local_scope.empty() && nodes.empty()) {
            throw node_loading_error(string("no child modules or nodes created from path: \"") + directory.data() + "\"");
        }

        mosaic_logger(log, "created node at " + string(directory));
    }

    node::node(node && other):
        nodes(move(other.nodes))
    {}


    module_view & node::get(const std::string & name) {
        return *global_scope.at(name);
    }

    node::~node() {
        forced_unload();
    }

    void node::unload(bool forced) {
        if (forced) {
            forced_unload();
        }
        else {
            for (auto & kv : nodes) {
                kv.second->unload();
            }
            nodes.clear();

            for (auto it = local_scope.begin(); it != local_scope.end(); it = local_scope.erase(it)) {
                auto & module_name      = it->first;
                auto & module_pointer   = it->second;

                module_pointer->unload();
                global_scope.erase(module_name);
            }
        }
    }

    void node::forced_unload() noexcept {
        for (auto & kv : nodes) {
            kv.second->forced_unload();
        }
        nodes.clear();

        for (auto it = local_scope.begin(); it != local_scope.end(); it = local_scope.erase(it)) {
            auto & module_name      = it->first;
            auto & module_pointer   = it->second;

            try {
                module_pointer->unload();
                global_scope.erase(module_name);
            } catch (error & e) {
                mosaic_logger(log, string("error during forced unload: ") + e.what());
            }
        }
    }

    void node::link() {
        for (auto & kv : local_scope) {
            kv.second->link(*this);
        }

        for (auto & kv : nodes) {
            kv.second->link();
        }
    }

    void node::load_native_module(const std::string_view & filename) {
        auto library_instance = shared_library(filename);
        auto module_pointer = make_shared<module>(move(library_instance));

        local_scope[module_pointer->name()] = module_pointer;
        global_scope[module_pointer->name()] = module_pointer;
    }
}

