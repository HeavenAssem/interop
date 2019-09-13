#pragma once

#include "caches.hpp"
#include "field_view.hpp"
#include "function_view.hpp"

#include <declarations.h>

#include <functional>

using namespace std::placeholders;

namespace interop::internals {

class object_interface_t {
  public:
    template <typename T>
    std::shared_ptr<T> get(const std::string_view &) const;

  private:
    virtual function_ptr_t get_function(const std::string_view & name) const = 0;
    virtual field_ptr_t get_field(const std::string_view & name) const       = 0;
};

template <>
inline function_ptr_t object_interface_t::get<function_view_t>(const std::string_view & name) const
{
    return get_function(name);
}

template <>
inline field_ptr_t object_interface_t::get<field_view_t>(const std::string_view & name) const
{
    return get_field(name);
}

template <typename BASE>
class object_caching_layer_t: public BASE {

    mutable named_entity_cache_t<function_view_t> function_cache;
    mutable named_entity_cache_t<field_view_t> field_cache;

    virtual function_ptr_t fetch_function(const std::string_view & name) const = 0;
    virtual field_ptr_t fetch_field(const std::string_view & name) const       = 0;

    function_ptr_t get_function(const std::string_view & name) const final
    {
        return function_cache.get(name, bind(&object_caching_layer_t::fetch_function, this, _1));
    }

    field_ptr_t get_field(const std::string_view & name) const final
    {
        return field_cache.get(name, bind(&object_caching_layer_t::fetch_field, this, _1));
    }

  public:
    using BASE::BASE;

    void unload()
    {

        function_cache.clear();
        field_cache.clear();
    }
};

} // namespace interop::internals
