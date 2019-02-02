//
// Created by islam on 26.10.17.
//

#include "function_view.h"
#include "platform.h"

#include <logger.h>

using namespace std;

namespace interop {

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

val_t function_view_t::ffi_call(arg_pack_t args) const
{
    if (platform_function) {
        return non_native_call(move(args));
    }

    for (size_t i = 0; i < args.size(); ++i) {
        args[i].convert_to(metadata.arguments[i].type);
    }

    if (bound_object) {
        return metadata.invoke(bound_object, move(args));
    }

    return metadata.invoke(metadata.pointer, move(args));
}

val_t function_view_t::non_native_call(arg_pack_t args) const
{
    assert(platform_function);

    return platform_function->call(move(args));
}
} // namespace interop
