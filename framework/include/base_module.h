#pragma once
//
// Created by islam on 08.04.18.
//


#include "module_view.h"

#include <unordered_map>


namespace interop {
    class module_context;

    class base_module_t: public module_view {
        mutable std::unordered_map<std::string, function_ptr_t> functions_cache;

    protected:
        module_metadata metadata;

    public:
        base_module_t();
        base_module_t(base_module_t &&) = default;

        virtual void link(module_context & context) const {};

        const module_metadata & get_metadata() const override final;
        function_ptr_t function(const std::string & name) override final;

        virtual void unload();

    protected:
        virtual function_ptr_t fetch_function(const std::string & name) = 0;
    };
}
