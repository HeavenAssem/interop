//
// Created by islam on 13.05.17.
//

#pragma once

#include "module_definitions.h"

#include "definitions.h"

#include <exceptions.h>

namespace interop {
    MOSAIC_DEFINE_CURRENT_ABI_VERSION

    extern "C" {
        module_metadata MOSAIC_MODULE_METADATA {};

        void MOSAIC_MODULE_INITIALIZE (module_context & context);
        void MOSAIC_MODULE_REGISTER ();
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

        template <class operation, class T, class ... Ts, class ... Args>
        static void iterate_impl(Args && ... args) {
            operation::template exec<T>(std::forward<Args>(args)...);
            if constexpr (sizeof...(Ts) > 0) {
                iterate_impl<operation, Ts...>(std::forward<Args>(args)...);
            }
        }

        template <class operation, class ... Ts>
        struct iterate {
            template<class ... Args>
            static void exec(Args && ... args) {
                if constexpr (sizeof...(Ts) > 0) {
                    iterate_impl<operation, Ts...>(std::forward<Args>(args)...);
                }
            }
        };

        struct describe_type_op {
            template <class T>
            static void exec(std::vector<type_metadata> & metadata) {
                metadata.push_back(describe_type<T>());
            }
        };

        template <class ... Ts>
        static void describe_types(std::vector<type_metadata> & metadata) {
            iterate<describe_type_op, Ts...>::exec(metadata);
        }
    }

    template<class R, class ... Args>
    void register_function(const std::string_view & name, R(*function)(Args ...)) {
        function_metadata metadata {
            .pointer     = (void *)function,
            .name        = name.data(),
            .return_type = details::describe_type<R>()
        };
        details::describe_types<Args ...>(metadata.arguments);

        _mosaic_module_metadata.functions.push_back(metadata);
    }

    template <class T, T> struct proxy;

    template <typename R, typename C, typename ...Args, R (C::*function)(Args...)>
    struct proxy<R (C::*)(Args...), function> {
        using Class = C;

        static R call(C * obj, Args &&... args) {
            return (obj->*function)(std::forward<Args>(args)...);
        }

        static std::vector<type_metadata> describe_args() {
            std::vector<type_metadata> arguments_metadata;
            details::describe_types<Args...>(arguments_metadata);
            return std::move(arguments_metadata);
        }

        static type_metadata describe_return_type() {
            return details::describe_type<R>();
        }
    };

    template <class C>
    struct destructor_proxy {
        using Class = C;

        static void call(C * object) {
            return delete object;
        }
    };

    template <class C>
    void register_destructor(object_metadata * metadata) {
        using proxy = destructor_proxy<C>;

        metadata->destructor.pointer = (void *)proxy::call;
    }

    template <typename C, typename ...Args>
    struct constructor_proxy {
        using Class = C;

        static C * call(Args &&... args) {
            return new C(std::forward<Args>(args)...);
        }

        static std::vector<type_metadata> describe_args() {
            std::vector<type_metadata> arguments_metadata;
            details::describe_types<Args...>(arguments_metadata);
            return std::move(arguments_metadata);
        }
    };

    template <class ... Args>
    struct constructor {
        template <class C> using proxy = constructor_proxy<C, Args...>;
    };

    template <class C>
    struct constructor_op {
        template <class Constructor>
        static void exec(std::vector<constructor_metadata> & metadata) {
            using proxy = typename Constructor::template proxy<C>;

            metadata.push_back(constructor_metadata{
                .pointer = (void *)proxy::call,
                .arguments = proxy::describe_args()
            });
        }
    };

    template<class C, class ... Constructors>
    void register_constructors(object_metadata * metadata) {
        details::iterate<constructor_op<C>, Constructors...>::exec(metadata->constructors);
    }

    template <class C>
    struct _helper {
        object_metadata * metadata;

        template <auto f>
        auto & method(const std::string & name) {
            using proxy = proxy<decltype(f), f>;
            static_assert(std::is_same<C, typename proxy::Class>::value, "Trying to add method from different class");

            metadata->methods.push_back(function_metadata{
                .pointer = (void *)proxy::call,
                .name = name,
                .return_type = proxy::describe_return_type(),
                .arguments = proxy::describe_args(),
            });

            return *this;
        }
    };

    template <class C, class ... Constructors>
    auto register_class(const std::string & name) {
        _helper<C> mt;
        _mosaic_module_metadata.types.push_back(object_metadata {
            .name = name
        });
        mt.metadata = &_mosaic_module_metadata.types.back();
        register_destructor<C>(mt.metadata);
        if constexpr (sizeof...(Constructors) > 0) {
            register_constructors<C, Constructors...>(mt.metadata);
        } else {
            register_constructors<C, constructor<>>(mt.metadata);
        }
        return mt;
    }
}

#define REGISTER_MEMBER(name, fn) register_function(name, proxy<decltype(fn), fn>::call);
#define REGISTER_FUNCTION(fn) interop::register_function(#fn, fn);
