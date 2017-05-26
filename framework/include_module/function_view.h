//
// Created by islam on 13.05.17.
//

#pragma once

#include <dyncall.h>


namespace mosaic {
    class function_view {
        DCCallVM * vm = nullptr;

        void initialize_dyncall() {
            if (!vm) {
                vm = dcNewCallVM(4096);
                dcMode(vm, DC_CALL_C_DEFAULT);
            }
        }
        void push_arg(double arg) {
            dcArgDouble(vm, arg);
        }
        void push_arg(int arg) {
            dcArgInt(vm, arg);
        }
        void push_arg(bool arg) {
            dcArgBool(vm, arg);
        }
        void push_arg(char arg) {
            dcArgChar(vm, arg);
        }
        void push_arg(float arg) {
            dcArgFloat(vm, arg);
        }
        void push_arg(long arg) {
            dcArgLong(vm, arg);
        }
        void push_arg(long long arg) {
            dcArgLongLong(vm, arg);
        }
        void push_arg(void * arg) {
            dcArgPointer(vm, arg);
        }
        void push_arg(short arg) {
            dcArgShort(vm, arg);
        }

        template <class Arg>
        void push_args(Arg arg) {
            push_arg(arg);
        }

        template <class Arg, class ... Args>
        void push_args(Arg arg, Args ... args) {
            push_args(arg);
            push_args(args...);
        }

        void * pointer;
    public:
        function_view(void * pointer): pointer(pointer) {}
        function_view(): pointer(nullptr) {}

        template <class ... Args>
        void call(Args ... args) {
            initialize_dyncall();
            if constexpr (sizeof...(Args) > 0) {
                dcReset(vm);
                push_args(args...);
            }
            dcCallVoid(vm, pointer);
            dcFree(vm);
            vm = nullptr;
        }
    };
}

