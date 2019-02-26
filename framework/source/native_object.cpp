//
// Created by islam on 26.10.17.
//

#include "native_object.hpp"
#include "function_view.hpp"
#include "internals/metadata_utils.hpp"

#include <algorithm>
#include <functional>

using namespace std;
using namespace literals;
using namespace placeholders;

namespace interop {

function_ptr_t native_object_t::function(const string_view & name) const
{
    return cache.get(name, [this](const string_view & name){
        auto & method_metadata = interop::internals::find_metadata(metadata.methods, name, [&] {
        throw function_lookup_error_t("method with name \""s + name.data() +
                                      "\" not found in object " + metadata.name);
    });

    return make_shared<function_view_t>(method_metadata, pointer);
    });
}

native_object_t::~native_object_t()
{
    reinterpret_cast<void (*)(void *)>(metadata.destructor.pointer)(pointer);
}
} // namespace interop
