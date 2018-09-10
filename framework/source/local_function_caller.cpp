//
// Created by islam on 27.05.17.
//

#include "local_function_caller.h"

// #include <dyncall.h>


// namespace interop {
//     void local_function_caller::push_arg(double arg) {
//         if (cast_argument(arg))
//             return;
//         dcArgDouble(static_cast<DCCallVM *>(vm), arg);
//         ++argument_index;
//     }

//     void local_function_caller::push_arg(int arg) {
//         if (cast_argument(arg))
//             return;
//         dcArgInt(static_cast<DCCallVM *>(vm), arg);
//         ++argument_index;
//     }

//     void local_function_caller::push_arg(bool arg) {
//         if (cast_argument(arg))
//             return;
//         dcArgBool(static_cast<DCCallVM *>(vm), arg);
//         ++argument_index;
//     }

//     void local_function_caller::push_arg(char arg) {
//         if (cast_argument(arg))
//             return;
//         dcArgChar(static_cast<DCCallVM *>(vm), arg);
//         ++argument_index;
//     }

//     void local_function_caller::push_arg(float arg) {
//         if (cast_argument(arg))
//             return;
//         dcArgFloat(static_cast<DCCallVM *>(vm), arg);
//         ++argument_index;
//     }

//     void local_function_caller::push_arg(long arg) {
//         if (cast_argument(arg))
//             return;
//         dcArgLong(static_cast<DCCallVM *>(vm), arg);
//         ++argument_index;
//     }

//     void local_function_caller::push_arg(long long arg) {
//         if (cast_argument(arg))
//             return;
//         dcArgLongLong(static_cast<DCCallVM *>(vm), arg);
//         ++argument_index;
//     }

//     void local_function_caller::push_arg(void * arg) {
//         if (cast_argument(arg))
//             return;
//         dcArgPointer(static_cast<DCCallVM *>(vm), arg);
//         ++argument_index;
//     }

//     void local_function_caller::push_arg(short arg) {
//         if (cast_argument(arg))
//             return;
//         dcArgShort(static_cast<DCCallVM *>(vm), arg);
//         ++argument_index;
//     }

//     void * local_function_caller::vm = nullptr;
//     uint32_t local_function_caller::count = 0;

//     local_function_caller::local_function_caller(const function_metadata * metadata) : metadata(metadata), argument_index(0) {
//         if (!vm) {
//             vm = dcNewCallVM(4096);
//             dcMode(static_cast<DCCallVM *>(vm), DC_CALL_C_DEFAULT);
//         }
//         ++count;
//     }

//     local_function_caller::~local_function_caller() {
//         if (--count == 0) {
//             dcFree(static_cast<DCCallVM *>(vm));
//         }
//     }

//     template <class Fn>
//     auto call(Fn call_function, uint8_t & index, void * vm, void * pointer) {
//         index = 0;
//         if constexpr (std::is_same<decltype(call_function(static_cast<DCCallVM *>(vm), pointer)), void>::value) {
//             call_function(static_cast<DCCallVM *>(vm), pointer);
//             dcReset(static_cast<DCCallVM *>(vm));
//         }
//         else {
//             auto return_value = call_function(static_cast<DCCallVM *>(vm), pointer);
//             dcReset(static_cast<DCCallVM *>(vm));
//             return return_value;
//         }
//     }

//     double local_function_caller::call_double() {
//         return call(dcCallInt, argument_index, vm, metadata->pointer);
//     }

//     int local_function_caller::call_int() {
//         return call(dcCallInt, argument_index, vm, metadata->pointer);
//     }

//     bool local_function_caller::call_bool() {
//         return call(dcCallBool, argument_index, vm, metadata->pointer);
//     }

//     char local_function_caller::call_char() {
//         return call(dcCallChar, argument_index, vm, metadata->pointer);
//     }

//     float local_function_caller::call_float() {
//         return call(dcCallFloat, argument_index, vm, metadata->pointer);
//     }

//     long local_function_caller::call_long() {
//         return call(dcCallLong, argument_index, vm, metadata->pointer);
//     }

//     long long local_function_caller::call_long_long() {
//         return call(dcCallLongLong, argument_index, vm, metadata->pointer);
//     }

//     void * local_function_caller::call_ptr() {
//         return call(dcCallPointer, argument_index, vm, metadata->pointer);
//     }

//     short local_function_caller::call_short() {
//         return call(dcCallShort, argument_index, vm, metadata->pointer);
//     }

//     void local_function_caller::call_void() {
//         return call(dcCallVoid, argument_index, vm, metadata->pointer);
//     }
// }
