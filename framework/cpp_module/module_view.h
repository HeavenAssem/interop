//
// Created by islam on 13.05.17.
//

#pragma once

#include "function_view.hpp"
#include "object_view.hpp"

#include "internals/metadata_utils.hpp"
#include "internals/native_module_data.hpp"

#include "definitions.h"
#include "module_metadata.h"

#include <declarations.h>
#include <functional>

namespace interop {
class module_view_t {
  protected:
    std::string name;
    internals::native_module_data_opt_t native;

    const object_metadata_t & get_object_metadata(const std::string_view & name) const
    {
        using namespace std::string_literals;

        interop_invariant_m(native, "get_object_metadata called for not native module");
        return internals::find_metadata(native->types, name, [&] {
            throw type_lookup_error_t("type with name \""s + name.data() +
                                      "\" not found in module \"" + get_name() + "\"");
        });
    }

  public:
    module_view_t(std::string name, internals::native_module_data_opt_t native = std::nullopt)
      : name(std::move(name))
      , native(std::move(native))
    {}

    const std::string & get_name() const { return name; }
    bool is_native() const { return native.has_value(); }

    template <typename... Args>
    inline object_ptr_t create(const std::string_view & name, Args &&... args)
    {
        using namespace std::string_literals;

        if (native) {
            const auto & metadata = get_object_metadata(name);

            auto constructor = details::function_cast<void *, Args...>(
                internals::find_first_with_args<Args...>(metadata.constructors, [&] {
                    throw function_lookup_error_t("no matching constructor for type "s +
                                                  name.data());
                }).pointer);

            return object_view_t::create(constructor(std::forward<Args>(args)...), metadata);
        } else {
            return create_dynamic(name, {std::forward<Args>(args)...});
        }
    }

    virtual function_ptr_t function(const std::string_view & name)                              = 0;
    virtual object_ptr_t create_dynamic(const std::string_view & name, arg_pack_t)              = 0;
    virtual void listen(const std::string_view & module_name, std::function<void()> && handler) = 0;
    virtual ~module_view_t() = default;
};
} // namespace interop
