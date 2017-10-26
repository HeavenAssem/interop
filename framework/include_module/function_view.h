//
// Created by islam on 13.05.17.
//

#pragma once

#include "function_metadata.h"
#include "object_view.h"

#include <memory>
#include <exceptions.h>

namespace mosaic {
    /**
     * @details: framework should be aware of every instance of function_view to be able to re-check all calls if metadata changes
     *           therefore, it is non - copyable.
     *           To store function_view use smart pointers.
     */
    class function_view {
        using metadata_checker = void(*)(const function_metadata & );

        template <class R, class ... Args>
        class signature_checker {
            template <int index, class Arg>
            static void check_args_types(const function_metadata & metadata) {
                const auto expected_type = metadata.arguments[index].type;
                constexpr auto passed_type = enumerate_type<Arg>();
                if (passed_type != expected_type) {
                    throw function_call_error("argument type mismatch when performing strict call on argument number " + std::to_string(index + 1) + ": expected - " + to_string(expected_type) + ", passed - " + to_string(passed_type));
                }
            }

            template <int index, class Arg1, class Arg2, class ... _Args>
            static void check_args_types(const function_metadata & metadata) {
                check_args_types<index, Arg1>(metadata);
                check_args_types<index + 1, Arg2, _Args ...>(metadata);
            }

            template <int index>
            static void check_args_types(const function_metadata & metadata) {}

            static void check_args(const function_metadata & metadata) {
                const auto argument_count = sizeof...(Args);
                if (argument_count != metadata.arguments.size()) {
                    throw function_call_error("argument count mismatch: expected: " + std::to_string(metadata.arguments.size()) + "; got: " + std::to_string(argument_count));
                }
                check_args_types<0, Args ...>(metadata);
            }

            static void check_return_type(const function_metadata & metadata) {
                if constexpr (!std::is_void<R>::value) {      // Allow to discard return value (when no return type specified on call - do not perform check).
                    const auto expected_type = metadata.return_type.type;
                    const auto passed_type = enumerate_type<R>();
                    if (passed_type != expected_type) {
                        throw function_call_error("return type mismatch when performing strict call : expected - " +
                                                  to_string(expected_type) + ", passed - " + to_string(passed_type));
                    }
                }
            }

        public:
            signature_checker(const function_metadata & metadata, std::vector<metadata_checker> & checkers) {
                checkers.push_back(&signature_checker::check_metadata);
                check_metadata(metadata);
            }

            static void check_metadata(const function_metadata & metadata) {
                check_args(metadata);
                check_return_type(metadata);
            }
        };

        std::vector<metadata_checker> metadata_checkers;
        const function_metadata & metadata;
        void * bound_object;

    public:
        explicit function_view(const function_metadata & metadata)
            : metadata(metadata)
            , bound_object(nullptr)
        {}

        function_view(object_view & object, const function_metadata & metadata)
            : metadata(metadata)
            , bound_object(object.get_pointer())
        {}

        function_view(const function_view &) = delete;

        /**
         * @brief: Fast, strict call. No implicit type casting.
         */
        template <class R = void, class ... Args>
        R call(Args ... args) {
            static signature_checker<R, Args...> checker_instance (metadata, metadata_checkers);   /** check call signature and register check
                                                                                                      * in case if metadata changes (on function's module reload / replace) */
            if (bound_object) {
                return reinterpret_cast<R (*)(void *, Args ...)>(metadata.pointer)(bound_object, args...);
            } else {
                return reinterpret_cast<R (*)(Args ...)>(metadata.pointer)(args...);
            }
        }
    };
}

