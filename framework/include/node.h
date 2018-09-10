#pragma once
//
// Created by islam on 07.05.17.
//


#include "module_context.h"
#include "declarations.h"

#include <unordered_map>
#include <memory>

#include <definitions.h>


namespace interop {
    struct node_configuration_t;
    struct native_module_configuration_t;

    class node_t : public module_context {
        // static std::unordered_map<std::string, std::shared_ptr<native_module_t>> global_scope;

        std::string name;
        std::unordered_map<std::string, std::unique_ptr<base_module_t>> local_scope;
        std::unordered_map<std::string, platform_ptr>            platforms;
    public:
        node_t(const node_configuration_t &);
        node_t(const node_t &) = delete;
        node_t(node_t &&);

        void link();


        module_view & get(const std::string & name) override;

        virtual ~node_t();

        void unload(bool forced = false);

    private:
        void load_native_module(const native_module_configuration_t &);

        void forced_unload() noexcept;
    };
}
