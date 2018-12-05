//
// Created by islam on 26.10.17.
//

#include "function_view.h"
#include "platform.h"

#include <logger.h>

#include <ffi.h>

using namespace std;

namespace interop {
namespace {
ffi_type & to_libffi_type(type_e type)
{
    static ffi_type ffi_type_string{sizeof(string), 0, FFI_TYPE_STRUCT, nullptr};

    switch (type) {
    case type_e::Void:
        return ffi_type_void;
    case type_e::Bool:
        return ffi_type_uint8;
    case type_e::Char:
        return ffi_type_sint8;
    case type_e::Short:
        return ffi_type_sint16;
    case type_e::Float:
        return ffi_type_float;
    case type_e::Double:
        return ffi_type_double;
    case type_e::Int:
        return ffi_type_sint;
    case type_e::Long:
        return ffi_type_slong;
    case type_e::LongLong:
        return ffi_type_slong;
    case type_e::Pointer:
        return ffi_type_pointer;
    case type_e::String:
        return ffi_type_string;
    default:
        throw type_mismatch_error_t("unsupported by libffi wrapper type");
    }
}
} // namespace

function_view_t::function_view_t(const function_metadata_t & metadata)
  : metadata(metadata)
  , bound_object(nullptr)
  , platform_function(nullptr)
{
    assert(!metadata.is_empty() && "create function_view_t from empty metadata");
}

function_view_t::function_view_t(object_view_t & object, const function_metadata_t & metadata)
  : function_view_t(metadata)
{
    bound_object = object.get_pointer();
}

function_view_t::function_view_t(const platform_function_ptr & function,
                                 const function_metadata_t & metadata)
  : function_view_t(metadata)
{
    platform_function = function;
}

val_t function_view_t::ffi_call(arg_pack_t args)
{
    if (!metadata.is_native()) {
        return non_native_call(std::move(args));
    }

    // checks
    if (args.size() != metadata.arguments.size()) {
        throw arguments_mismatch_error_t("at ffi call: arguments count mismatch: expected " +
                                         std::to_string(metadata.arguments.size()) + ", got " +
                                         std::to_string(args.size()));
    }

    ffi_cif cif;

    if (bound_object) {
        const size_t argument_count = args.size() + 2;
        ffi_type * ffi_arg_types[10];  // FIXME: do something!
        {
            size_t arg_index           = 0;
            ffi_arg_types[arg_index++] = &ffi_type_pointer;

            for (const auto & argument_metadata : metadata.arguments) {
                ffi_arg_types[arg_index++] = &to_libffi_type(argument_metadata.type);
            }
            ffi_arg_types[arg_index] = NULL;

            /* Initialize the cif */
            assert(ffi_prep_cif(&cif, FFI_DEFAULT_ABI, argument_count - 1,
                                &to_libffi_type(metadata.return_type.type),
                                ffi_arg_types) == FFI_OK);
        }

        void * ffi_args[10];  // FIXME: do something!
        {
            size_t arg_index      = 0;
            ffi_args[arg_index++] = &bound_object;

            for (auto & argument : args) {
                ffi_args[arg_index++] = argument.data();
            }
            ffi_args[arg_index] = NULL;
        }

        auto ret = type_subsystem::initialized_val(metadata.return_type.type);
        if (!ret.is<int>()) {
            throw runtime_error("this is bad");
        }
        ::ffi_call(&cif, FFI_FN(metadata.pointer), ret.data(), ffi_args);

        return ret;
    } else {
        if (metadata.context) {
            return {};
        } else {
            return {};
        }
    }
}

val_t function_view_t::non_native_call(arg_pack_t args)
{
    assert(platform_function);

    return platform_function->call(move(args));
}
} // namespace interop
