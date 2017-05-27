//
// Created by islam on 13.05.17.
//

#pragma once

#include "module_definitions.h"

#include "definitions.h"

#include <exceptions.h>

namespace mosaic {
    extern "C" {
        module_metadata _mosaic_module_metadata {};

        void _mosaic_module_initialize(module_context & context);
        void _mosaic_module_register();
    }

    namespace details {

        template<class T>
        type_metadata describe_type() {
            return type_metadata {
                .type = enumerate_type<T>(),
                .size = sizeof(T)
            };
        }

        template<>
        type_metadata describe_type<void>() {
            return type_metadata {
                .type = enumerate_type<void>(),
                .size = 0
            };
        }

        template<class T, class ... Ts>
        static void describe_types(std::vector<type_metadata> & metadata) {
            metadata.push_back(describe_type<T>());
            if constexpr (sizeof...(Ts) > 0) {
                describe_types<Ts ...>(metadata);
            }
        }
    }

    template<class R, class ... Args>
    void register_function(const std::string_view & name, R(*function)(Args ...)) {
        function_metadata metadata {
            .pointer     = reinterpret_cast<void *>(function),
            .name        = name.to_string(),
            .return_type = details::describe_type<R>()
        };
        if constexpr (sizeof...(Args) > 0) {
            details::describe_types<Args ...>(metadata.arguments);
        }
        _mosaic_module_metadata.functions.push_back(metadata);
    }
}

#define REGISTER_FUNCTION(fn) mosaic::register_function(#fn, fn);
