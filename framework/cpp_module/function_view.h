//
// Created by islam on 13.05.17.
//

#pragma once

#include "function_metadata.h"
#include "object_view.h"

#include <memory>
#include <exceptions.h>
#include <cassert>
#include <any>

namespace mosaic {
    namespace detail {
        using metadata_checker = void(*)(const function_metadata & );

        template <class R, class ... Args>
        class signature_checker {
            template <int index, class Arg>
            static void check_args_types(const std::vector<type_metadata> & metadata) {
                const auto expected_type = metadata[index].type;
                constexpr auto passed_type = enumerate_type<Arg>();
                if (passed_type != expected_type) {
                    throw arguments_mismatch_error("argument type mismatch when performing strict call on argument number " + std::to_string(index + 1) + ": expected - " + to_string(expected_type) + ", passed - " + to_string(passed_type));
                }
            }

            template <int index, class Arg1, class Arg2, class ... _Args>
            static void check_args_types(const std::vector<type_metadata> & metadata) {
                check_args_types<index, Arg1>(metadata);
                check_args_types<index + 1, Arg2, _Args ...>(metadata);
            }

            template <int>
            static void check_args_types(const std::vector<type_metadata> & metadata) {}

        public:
            static void check_args(const std::vector<type_metadata> & metadata) {
                const auto argument_count = sizeof...(Args);
                if (argument_count != metadata.size()) {
                    throw arguments_mismatch_error("argument count mismatch: expected: " + std::to_string(metadata.size()) + "; got: " + std::to_string(argument_count));
                }
                check_args_types<0, Args ...>(metadata);
            }

            static void check_return_type(const type_metadata & metadata) {
                if constexpr (!std::is_void<R>::value) {      // Allow to discard return value (when no return type specified on call - do not perform check).
                    const auto & expected_type = metadata.type;
                    const auto & passed_type = enumerate_type<R>();
                    if (passed_type != expected_type) {
                        throw function_call_error("return type mismatch when performing strict call : expected - " +
                                                  to_string(expected_type) + ", passed - " + to_string(passed_type));
                    }
                }
            }


            signature_checker(const function_metadata & metadata, std::vector<metadata_checker> & checkers) {
                checkers.push_back(&signature_checker::check_metadata);
                check_metadata(metadata);
            }

            static void check_metadata(const function_metadata & metadata) {
                if (metadata.is_native()) {
                    check_args(metadata.arguments);
                    check_return_type(metadata.return_type);
                }
            }
        };
    }

    /**
     * @details: framework should be aware of every instance of function_view to be able to re-check all calls if metadata changes
     *           therefore, it is non - copyable.
     *           To store function_view use function_ptr.
     */
    class function_view {
        std::vector<detail::metadata_checker>   metadata_checkers;
        function_metadata                       metadata;
        void *                                  bound_object;
        platform_function_ptr                   platform_function;

    public:
        explicit function_view(const function_metadata & metadata);
        function_view(object_view & object, const function_metadata & metadata);
        function_view(const platform_function_ptr & function, const function_metadata & metadata);
        function_view(const function_view &) = delete;

        /**
         * @brief: Fast, strict call. No implicit type casting.
         */
        template <class R = void, class ... Args>
        R call(Args && ... args) {
            static detail::signature_checker<R, Args...> checker_instance (metadata, metadata_checkers);   /** check call signature and register check
                                                                                                      * in case if metadata changes (on function's module reload / replace) */
            if (metadata.is_native()) {
                if (bound_object) {
                    return reinterpret_cast<R (*)(void *, Args ...)>(metadata.pointer)(bound_object,
                                                                                       std::forward<Args>(args)...);
                } else {
                    return reinterpret_cast<R (*)(Args ...)>(metadata.pointer)(std::forward<Args>(args)...);
                }
            } else {
                assert(platform_function);

                if constexpr (std::is_void<R>::value) {
                    non_native_call(std::vector<std::any> { std::forward<Args>(args)... });
                    return R();
                } else {
                    return std::any_cast<R>(non_native_call(std::vector<std::any> {std::forward<Args>(args)...}));
                }
            }
        }

    private:
        std::any non_native_call(std::vector<std::any> && args);
    };
}
