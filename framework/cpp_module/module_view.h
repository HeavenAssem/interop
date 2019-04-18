//
// Created by islam on 13.05.17.
//

#pragma once

#include "function_view.hpp"
#include "object_view.hpp"

#include "internals/metadata_utils.hpp"

#include "definitions.h"
#include "module_metadata.h"

#include <declarations.h>
#include <functional>

namespace interop {
class module_view_t {
  protected:
    virtual const module_metadata_t & get_metadata() const = 0;

  public:
    virtual function_ptr_t function(const std::string_view & name) = 0;

    inline const object_metadata_t & get_object_metadata(const std::string_view & name) const
    {
        using namespace std::string_literals;

        auto & metadata = get_metadata();
        return internals::find_metadata(metadata.types, name, [&] {
            throw type_lookup_error_t("type with name \""s + name.data() +
                                      "\" not found in module \"" + metadata.name + "\"");
        });
    }

    template <typename... Args>
    inline object_ptr_t create(const std::string_view & name, Args &&... args) const
    {
        using namespace std::string_literals;

        auto & metadata  = get_object_metadata(name);
        auto constructor = details::function_cast<void *, Args...>(
            internals::find_first_with_args<Args...>(metadata.constructors, [&] {
                throw function_lookup_error_t("no matching constructor for type "s + name.data());
            }).pointer);

        return object_view_t::create(constructor(std::forward<Args>(args)...), metadata);
    }

    virtual object_ptr_t create_dynamic(const std::string_view & name, arg_pack_t)              = 0;
    virtual void listen(const std::string_view & module_name, std::function<void()> && handler) = 0;
    virtual const std::string & name() const                                                    = 0;
    virtual ~module_view_t() = default;
};
} // namespace interop
