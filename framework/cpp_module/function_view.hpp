//
// Created by islam on 13.05.17.
//

#pragma once

#include "function_metadata.h"
#include "object_view.hpp"

#include "internals/strict_call_validator.h"

#include <exceptions.h>
#include <invariant.h>

#include <memory>
#include <optional>

namespace interop {
namespace details {
template <typename R, typename... Args>
auto function_cast(void * pointer)
{
    return reinterpret_cast<R (*)(typename std::decay<Args>::type...)>(pointer);
}
} // namespace details

/**
 * @details: framework should be aware of every instance of function_view_t to
 * be able to re-check
 * all calls if metadata changes therefore, it is non - copyable. To store
 * function_view_t use
 * function_ptr.
 */
struct function_view_t {
    const std::string name;
    const internals::native_callable_data_opt_t native;

    function_view_t(std::string name, internals::native_callable_data_opt_t native = std::nullopt)
      : name(std::move(name))
      , native(std::move(native))
    {}

    function_view_t(const function_view_t &) = delete;
    function_view_t(function_view_t &&)      = delete;

    /**
     * @brief Fast, strict, native to native call with NO implicit type casting
     *
     * @details Can fallback to dynamic call if static is not posiible
     */
    template <class R = void, class... Args>
    R call(Args &&... args)
    {
        /** check call signature and register check
         * in case if metadata changes (on function's module
         * reload / replace) */
        static internals::strict_call_validator_t<R, Args...> checker_instance(name, native);

        return native ? native->context
                            ? details::function_cast<R, void *, Args...>(native->pointer)(
                                  native->context, std::forward<Args>(args)...)
                            : details::function_cast<R, Args...>(native->pointer)(
                                  std::forward<Args>(args)...)
                      : call_dynamic(arg_pack_t{std::forward<Args>(args)...}).as<R>();
    }

    /**
     * @brief flexible, dynamic, native to (non-)native call with runtime implicit type casting
     */
    virtual val_t call_dynamic(arg_pack_t = {}) const = 0;

    /**
     * @brief destructor
     */
    virtual ~function_view_t() = default;

    /**
     * @brief imitator interface
     *
     * @details mocks native callable and allows compile-time implicit type casting
     *
     * @tparam T
     */
    template <typename T>
    friend struct imitator_t;

    template <typename T>
    struct imitator_t;

    template <typename R, typename C, typename... Args>
    struct imitator_t<R (C::*)(Args...)> {
        std::shared_ptr<function_view_t> function_view;

        inline R operator()(Args... args) { return function_view->call<R>(args...); }
    };

    template <typename R, typename... Args>
    struct imitator_t<R(Args...)> {
        std::shared_ptr<function_view_t> function_view;

        inline R operator()(Args... args) { return function_view->call<R>(args...); }
    };

    template <typename T>
    imitator_t<T> as()
    {
        return {*this};
    }
}; // namespace interop

} // namespace interop
