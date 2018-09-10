//
// Created by islam on 26.10.17.
//

#include "function_view.h"
#include "platform.h"

using namespace std;

namespace interop {
    function_view::function_view(const function_metadata & metadata)
        : metadata(metadata)
        , bound_object(nullptr)
        , platform_function(nullptr)
    {
        assert(!metadata.is_empty() && "create function_view from empty metadata");
    }

    function_view::function_view(object_view & object, const function_metadata & metadata)
        : function_view(metadata)
    {
        bound_object = object.get_pointer();
    }

    function_view::function_view(const platform_function_ptr & function, const function_metadata & metadata)
        : function_view(metadata)
    {
        platform_function = function;
    }

    std::any function_view::non_native_call(arg_pack_t && args) {
        assert(platform_function);

        return platform_function->call(move(args));
    }
}
