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

        template <class T>
        T call_function_selector();

        std::unique_ptr<function_caller> caller;
    public:
        function_view(std::unique_ptr<function_caller> && caller): caller(std::move(caller)) {}
        function_view(): caller(nullptr) {}

        template <class R = void, class ... Args>
        R call(Args ... args) {
            if constexpr (sizeof...(Args) > 0) {
                push_args(args...);
            }
            return call_function_selector<R>();
        }
    };

#define MAP_FUNCTION_CALL(T, F) template <> inline T function_view::call_function_selector <T> () {\
    using RetType = decltype(caller->F());\
    if constexpr (std::is_same<T, RetType>::value) {\
        return caller->F();\
    }\
    else if constexpr (std::is_convertible<T, RetType>::value) {\
        return static_cast<T>(caller->F());\
    }\
    static_assert(std::is_convertible<T, RetType>::value, "Cannot map function to type: return type is not convertible");\
}

    template <class T> inline T function_view::call_function_selector () {
        if constexpr (std::is_pointer<T>::value) {
            return static_cast<T>(caller->call_ptr());
        }
        static_assert(std::is_pointer<T>::value, "No function call for specified type");
    }

    MAP_FUNCTION_CALL(double, call_double)
    MAP_FUNCTION_CALL(int   , call_int)
    MAP_FUNCTION_CALL(bool  , call_bool)
    MAP_FUNCTION_CALL(char  , call_char)
    MAP_FUNCTION_CALL(float , call_float)
    MAP_FUNCTION_CALL(long  , call_long)
    MAP_FUNCTION_CALL(long long , call_long_long)
    MAP_FUNCTION_CALL(void  , call_void)
    MAP_FUNCTION_CALL(short , call_short)

#undef MAP_FUNCTION_CALL
}

