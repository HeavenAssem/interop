//
// Created by islam on 26.10.17.
//

#include "native_object.hpp"
#include "field_view.hpp"
#include "internals/metadata_utils.hpp"
#include "native_function.hpp"

#include <logger.hpp>

#include <algorithm>
#include <functional>

using namespace std;
using namespace literals;
using namespace placeholders;

namespace interop {

const std::string & native_object_t::name() const { return metadata.name; }

function_ptr_t native_object_t::fetch_function(const string_view & name) const
{
    if (auto method_metadata = interop::internals::find_metadata_opt(metadata.methods, name)) {
        return make_shared<native_function_t>(*method_metadata, pointer);
    }
    throw lookup_error_t("function with name \""s + name.data() + "\" not found in object" +
                         this->name());
}

field_ptr_t native_object_t::fetch_field(const string_view & name) const
{
    if (auto field_metadata = interop::internals::find_metadata_opt(metadata.fields, name)) {
        return make_shared<field_view_t>(pointer, *field_metadata);
    }

    throw lookup_error_t("field with name \""s + name.data() + "\" not found in object" +
                         this->name());
}

native_object_t::~native_object_t()
{
    metadata.destructor(pointer);
    interop_logger(debug, "destroyed obj");
}
} // namespace interop
