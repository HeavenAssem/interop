#pragma once
//
// Created by islam on 07.05.17.
//


#include "module_context.h"

#include <unordered_map>
#include <memory>

#include <definitions.h>


namespace mosaic {
    class module;

    class node : public module_context {
        static std::unordered_map<std::string, std::shared_ptr<module>> global_scope;

        std::unordered_map<std::string, std::shared_ptr<module>> local_scope;
        std::unordered_map<std::string, std::unique_ptr<node>>   nodes;
    public:
        node(const std::string_view & directory);
        node(const node &) = delete;
        node(node &&);

        void link();


        module_view & get(const std::string & name) override;


        virtual ~node();

        void unload(bool forced = false);

    private:
        void load_native_module(const std::string_view & filename);

        void forced_unload() noexcept;
    };
}


