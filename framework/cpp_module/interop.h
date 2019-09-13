//
// Created by islam on 13.05.17.
//

#pragma once

#include "definitions.h"
#include "internals/class_reflection.h"
#include "module_definitions.h"
#include "type_subsystem/user_type_registry.hpp"

#include <exceptions.h>

namespace interop {
INTEROP_DEFINE_CURRENT_ABI_VERSION

extern "C" {
module_metadata_t interop_module_metadata_m{};

void interop_link(module_context_t & context);
void interop_register();
}

template <typename Function>
void register_function(std::string name, Function function)
{
    using internals::function_reflection::function_reflector_t;
    using internals::function_reflection::functor_reflector_t;

    if constexpr (std::is_class<Function>::value) {
        interop_module_metadata_m.functions.push_back(
            functor_reflector_t::reflect(std::move(name), std::function{std::move(function)}));
    } else {
        interop_module_metadata_m.functions.push_back(
            function_reflector_t<Function>::reflect(std::move(name), std::move(function)));
    }
}

template <typename... Args>
using constructor = internals::class_reflection::constructor_t<Args...>;

template <class Class, class... Constructors>
auto register_class(std::string name)
{
    using internals::class_reflection::class_reflector_t;

    user_type_registry_t::set<Class>();

    interop_module_metadata_m.types.emplace_back(std::move(name));
    return class_reflector_t<Class, Constructors...>(interop_module_metadata_m.types.back());
}
} // namespace interop
