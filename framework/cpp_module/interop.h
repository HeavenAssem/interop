//
// Created by islam on 13.05.17.
//

#pragma once

#include "definitions.h"
#include "inner/class_reflection.h"
#include "module_definitions.h"

#include <exceptions.h>

namespace interop {
INTEROP_DEFINE_CURRENT_ABI_VERSION

extern "C" {
module_metadata_t INTEROP_MODULE_METADATA{};

void INTEROP_MODULE_INITIALIZE(module_context_t & context);
void INTEROP_MODULE_REGISTER();
}

template <typename Function>
void register_function(std::string name, Function function)
{
    using internals::function_reflection::function_reflector_t;
    using internals::function_reflection::functor_reflector_t;

    if constexpr (std::is_class<Function>::value) {
        INTEROP_MODULE_METADATA.functions.push_back(
            functor_reflector_t::reflect(std::move(name), std::function{std::move(function)}));
    } else {
        INTEROP_MODULE_METADATA.functions.push_back(
            function_reflector_t<Function>::reflect(std::move(name), std::move(function)));
    }
}

template <typename... Args>
using constructor = internals::class_reflection::constructor_t<Args...>;

template <class Class, class... Constructors>
auto register_class(std::string name)
{
    using internals::class_reflection::class_reflector_t;

    INTEROP_MODULE_METADATA.types.emplace_back(std::move(name));
    return class_reflector_t<Class, Constructors...>(INTEROP_MODULE_METADATA.types.back());
}
} // namespace interop
