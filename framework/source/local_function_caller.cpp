//
// Created by islam on 27.05.17.
//

#include "local_function_caller.h"

#include <dyncall.h>


namespace mosaic {
    void local_function_caller::push_arg(double arg) {
        if (cast_argument(arg))
            return;
        dcArgDouble(static_cast<DCCallVM *>(vm), arg);
        ++argument_index;
    }

    void local_function_caller::push_arg(int arg) {
        if (cast_argument(arg))
            return;
        dcArgInt(static_cast<DCCallVM *>(vm), arg);
        ++argument_index;
    }

    void local_function_caller::push_arg(bool arg) {
        if (cast_argument(arg))
            return;
        dcArgBool(static_cast<DCCallVM *>(vm), arg);
        ++argument_index;
    }

    void local_function_caller::push_arg(char arg) {
        if (cast_argument(arg))
            return;
        dcArgChar(static_cast<DCCallVM *>(vm), arg);
        ++argument_index;
    }

    void local_function_caller::push_arg(float arg) {
        if (cast_argument(arg))
            return;
        dcArgFloat(static_cast<DCCallVM *>(vm), arg);
        ++argument_index;
    }

    void local_function_caller::push_arg(long arg) {
        if (cast_argument(arg))
            return;
        dcArgLong(static_cast<DCCallVM *>(vm), arg);
        ++argument_index;
    }

    void local_function_caller::push_arg(long long arg) {
        if (cast_argument(arg))
            return;
        dcArgLongLong(static_cast<DCCallVM *>(vm), arg);
        ++argument_index;
    }

    void local_function_caller::push_arg(void * arg) {
        if (cast_argument(arg))
            return;
        dcArgPointer(static_cast<DCCallVM *>(vm), arg);
        ++argument_index;
    }

    void local_function_caller::push_arg(short arg) {
        if (cast_argument(arg))
            return;
        dcArgShort(static_cast<DCCallVM *>(vm), arg);
        ++argument_index;
    }

    void * local_function_caller::vm = nullptr;
    uint32_t local_function_caller::count = 0;

    local_function_caller::local_function_caller(const function_metadata * metadata) : metadata(metadata), argument_index(0) {
        if (!vm) {
            vm = dcNewCallVM(4096);
            dcMode(static_cast<DCCallVM *>(vm), DC_CALL_C_DEFAULT);
        }
        ++count;
    }

    local_function_caller::~local_function_caller() {
        if (--count == 0) {
            dcFree(static_cast<DCCallVM *>(vm));
        }
    }

    void local_function_caller::call() {
        argument_index = 0;
        dcCallVoid(static_cast<DCCallVM *>(vm), metadata->pointer);
        dcReset(static_cast<DCCallVM *>(vm));
    }
}
