#include "native_function.hpp"

namespace interop {

template <typename T>
T one_of(T one, T other)
{
    return one ? one : other;
}

template <typename T>
T only_one_of(T one, T other, const char * msg)
{
    interop_invariant_m(static_cast<bool>(one) + static_cast<bool>(other) < 2, msg);
    return one_of(one, other);
}

native_function_t::native_function_t(const function_metadata_t & metadata, void * object)
  : function_view_t{metadata.name,
                    {{metadata.pointer,
                      only_one_of(metadata.context, object,
                                  "attempt to bind callable with context to object"),
                      metadata.arguments, metadata.return_type}}}
  , dynamic_invoke{metadata.invoke}
{}

native_function_t::native_function_t(const constructor_metadata_t & metadata,
                                     std::string object_name)
  : function_view_t{std::move(object_name),
                    {{metadata.pointer,
                      nullptr,
                      metadata.arguments,
                      {type_e::Pointer, sizeof(void *)}}}}
  , dynamic_invoke{metadata.invoke}
{}

val_t native_function_t::call_dynamic(arg_pack_t args) const
{
    interop_invariant_m(native, "no native data inside native_function_t");

    return call_dynamic(dynamic_invoke, one_of(native->context, native->pointer), std::move(args),
                        native->arguments);
}

val_t native_function_t::call_dynamic(universal_wrapper_t invoke, void * context, arg_pack_t args,
                                      const std::vector<type_metadata_t> & args_meta)
{
    for (size_t i = 0; i < args.size(); ++i) {
        args[i].convert_to(args_meta[i].type);
    }

    return invoke(context, move(args));
}

} // namespace interop
