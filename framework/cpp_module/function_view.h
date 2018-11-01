//
// Created by islam on 13.05.17.
//

#pragma once

#include "function_metadata.h"
#include "object_view.h"

#include "internals/strict_call_validator.h"

#include <any>
#include <cassert>
#include <exceptions.h>
#include <memory>

#include <iostream>

namespace interop {

/**
 * @details: framework should be aware of every instance of function_view_t to be able to re-check
 * all calls if metadata changes therefore, it is non - copyable. To store function_view_t use
 * function_ptr.
 */
class function_view_t {
    std::vector<internals::metadata_checker_t> metadata_checkers;
    function_metadata_t metadata;
    void * bound_object;
    platform_function_ptr platform_function;

  public:
    explicit function_view_t(const function_metadata_t & metadata);
    function_view_t(object_view_t & object, const function_metadata_t & metadata);
    function_view_t(const platform_function_ptr & function, const function_metadata_t & metadata);
    function_view_t(const function_view_t &) = delete;

    /**
     * @brief: Fast, strict call. No implicit type casting.
     */
    template <class R = void, class... Args>
    R call(Args &&... args)
    {
        static internals::strict_call_validator_t<R, Args...> checker_instance(
            metadata, metadata_checkers); /** check call signature and register check
                                           * in case if metadata changes (on function's module
                                           * reload / replace) */
        if (metadata.is_native()) {
            if (bound_object) {
                return reinterpret_cast<R (*)(void *, Args...)>(metadata.pointer)(
                    bound_object, std::forward<Args>(args)...);
            } else {
                if (metadata.context) {
                    return reinterpret_cast<R (*)(void *, Args...)>(metadata.pointer)(
                        metadata.context, std::forward<Args>(args)...);
                } else {
                    return reinterpret_cast<R (*)(Args...)>(metadata.pointer)(
                        std::forward<Args>(args)...);
                }
            }
        } else {
            assert(platform_function);

            if constexpr (std::is_void<R>::value) {
                non_native_call(arg_pack_t{std::forward<Args>(args)...});
                return;
            } else {
                auto return_value = non_native_call(arg_pack_t{std::forward<Args>(args)...});
                if (return_value.type() != typeid(R)) {
                    throw interop::type_mismatch_error_t(
                        "return type mismatch"); // TODO: improve error message
                }
                return std::any_cast<R>(return_value);
            }
        }
    }

    template <typename T>
    friend struct imitator_t;

    template <typename T>
    struct imitator_t;

    template <typename R, typename C, typename... Args>
    struct imitator_t<R (C::*)(Args...)> {
        function_view_t & function_view;

        inline R operator()(Args... args) {
            //std::cout << "!!!: " << sizeof...(Args) << std::endl;
            auto r = function_view.call<R>(args...);
            std::cout << "!!!: " << r << std::endl;
            return r;
        }
    };

    template <typename R, typename... Args>
    struct imitator_t<R(Args...)> {
        function_view_t & function_view;

        inline R operator()(Args... args) { return function_view.call<R>(args...); }
    };

    template <typename T>
    imitator_t<T> as()
    {
        return {*this};
    }

  private:
    std::any non_native_call(arg_pack_t args);
};
} // namespace interop
