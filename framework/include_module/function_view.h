//
// Created by islam on 13.05.17.
//

#pragma once

#include "function_caller.h"

#include <memory>

namespace mosaic {
    class function_view {
        template <class Arg>
        void push_args(Arg arg) {
            caller->push_arg(arg);
        }

        template <class Arg, class ... Args>
        void push_args(Arg arg, Args ... args) {
            push_args(arg);
            push_args(args...);
        }

        std::unique_ptr<function_caller> caller;
    public:
        function_view(std::unique_ptr<function_caller> && caller): caller(std::move(caller)) {}
        function_view(): caller(nullptr) {}

        template <class ... Args>
        void call(Args ... args) {
            if constexpr (sizeof...(Args) > 0) {
                push_args(args...);
            }
            caller->call();
        }
    };
}

