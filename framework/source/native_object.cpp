//
// Created by islam on 26.10.17.
//

#include "native_object.hpp"
#include "internals/metadata_utils.hpp"
#include "native_function.hpp"

#include <algorithm>
#include <functional>

using namespace std;
using namespace literals;
using namespace placeholders;

namespace interop {

const std::string & native_object_t::name() const { return metadata.name; }

function_ptr_t native_object_t::function(const string_view & name) const
{
    return cache.get(name, [this](const string_view & name) {
        auto & method_metadata = interop::internals::find_metadata(metadata.methods, name, [&] {
            throw function_lookup_error_t("method with name \""s + name.data() +
                                          "\" not found in object " + metadata.name);
        });

        return make_shared<native_function_t>(method_metadata, pointer);
    });
}

native_object_t::~native_object_t() { metadata.destructor(pointer); }
} // namespace interop
