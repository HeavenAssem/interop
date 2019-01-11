#include "function_view.h"

#include <exceptions.h>

#include <ffi.h>

#include <string>

using namespace std;

namespace interop {
namespace {

inline ffi_type & to_libffi_type(type_e type)
{
    // FIXME: this probably is shit
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

inline val_t ffi_call(const function_view_t & func, arg_pack_t args)
{
    const auto& metadata = func.metadata;
    const auto& bound_object = func.bound_object;

    if (!func.metadata.is_native()) {
        return func.non_native_call(std::move(args));
    }

    // checks
    if (args.size() != metadata.arguments.size()) {
        throw arguments_mismatch_error_t("at ffi call: arguments count mismatch: expected " +
                                         std::to_string(metadata.arguments.size()) + ", got " +
                                         std::to_string(args.size()));
    }

    assert(!(bound_object && metadata.context));

    void * context = bound_object ? bound_object : metadata.context ? metadata.context : nullptr;

    ffi_cif cif;
    const size_t argument_count = args.size() + static_cast<bool>(context) + 1; // +1 for NULL

    ffi_type * ffi_arg_types[argument_count]; // TODO: replace with alloca()
    {
        size_t arg_index = 0;

        if (context) {
            ffi_arg_types[arg_index++] = &ffi_type_pointer;
        }

        for (const auto & argument_metadata : metadata.arguments) {
            ffi_arg_types[arg_index++] = &to_libffi_type(argument_metadata.type);
        }
        ffi_arg_types[arg_index++] = NULL;
        assert(arg_index == argument_count);

        /* Initialize the cif */
        assert(ffi_prep_cif(&cif, FFI_DEFAULT_ABI, argument_count - 1,
                            &to_libffi_type(metadata.return_type.type), ffi_arg_types) == FFI_OK);
    }

    // perform implicit conversions
    for (size_t i = 0; i < args.size(); ++i) {
        args[i].convert_to(metadata.arguments[i].type);
    }

    void * ffi_args[argument_count];
    {
        size_t arg_index = 0;

        if (context) {
            ffi_args[arg_index++] = &context;
        }

        for (auto & argument : args) {
            ffi_args[arg_index++] = argument.data();
        }
        ffi_args[arg_index++] = NULL;
        assert(arg_index == argument_count);
    }

    auto ret = val_t{metadata.return_type.type};
    ::ffi_call(&cif, FFI_FN(metadata.pointer), ret.data(), ffi_args);
    return ret;
}

} // namespace interop
