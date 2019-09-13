#pragma once

#include "field_metadata.hpp"
#include "function_view.hpp"
#include "internals/native_field_data.hpp"
#include "property_view.hpp"
#include "type_subsystem/type.h"

#include <exceptions.h>

namespace interop {

class field_view_t: public property_view_t {
    const std::string name_;
    const internals::native_field_runtime_data_opt_t native;

  protected:
    explicit field_view_t(std::string);

  public:
    explicit field_view_t(void * object, const field_metadata_t &);

    const std::string & name() const override;

    template <typename T>
    T get() const
    {
        if (native) {
            if (auto type = enumerate_type<T>(); type != native->meta.type) {
                throw type_mismatch_error_t("unable to get value of type " +
                                            to_string(native->meta.type) + " as " +
                                            to_string(type));
            }

            return details::function_cast<T>(native->meta.getter)(native->object);
        }

        return get_dynamic().as<T>();
    }

    template <typename T>
    void set(T && value) const
    {
        if (native) {
            if (auto type = enumerate_type<T>(); type != native->meta.type) {
                throw type_mismatch_error_t("unable to set value of type " + to_string(type) +
                                            " to " + to_string(native->meta.type));
            }

            return details::function_cast<T>(native->meta.setter)(native->object,
                                                                  std::forward<T>(value));
        }

        return set_dynamic({std::forward<T>(value)});
    }

    val_t get_dynamic() const;
    void set_dynamic(val_t);

  private:
    virtual val_t get_dynamic_non_native() const;
    virtual void set_dynamic_non_native(val_t);
};

} // namespace interop
