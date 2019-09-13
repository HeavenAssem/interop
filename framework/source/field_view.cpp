#include "field_view.hpp"

namespace interop {

field_view_t::field_view_t(std::string name)
  : name_(std::move(name))
{}

field_view_t::field_view_t(void * object, const field_metadata_t & metadata)
  : name_(metadata.name)
  , native(internals::native_field_runtime_data_t{metadata, object})
{}

const std::string & field_view_t::name() const { return name_; }

val_t field_view_t::get_dynamic() const
{
    if (native) {
        return native->meta.dynamic_getter(native->object);
    }

    return get_dynamic_non_native();
}

void field_view_t::set_dynamic(val_t value)
{
    if (native) {
        return native->meta.dynamic_setter(native->object, std::move(value));
    }

    return set_dynamic_non_native(std::move(value));
}

val_t field_view_t::get_dynamic_non_native() const
{
    throw not_implemented("no getter for non-native property");
}

void field_view_t::set_dynamic_non_native(val_t)
{
    throw not_implemented("no setter for non-native property");
}

} // namespace interop
