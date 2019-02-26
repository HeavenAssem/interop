#pragma once
//
// Created by islam on 08.04.18.
//

#include "module_view.h"
#include "caches.hpp"

namespace interop {
class node_t;

class base_module_t: public module_view_t {
    mutable function_cache_t cache;

  protected:
    module_metadata_t metadata;

  public:
    base_module_t();
    base_module_t(base_module_t &&) = default;

    virtual void link(node_t &) const {};

    const module_metadata_t & get_metadata() const override final;
    function_ptr_t function(const std::string_view & name) override final;

    virtual void unload();

  protected:
    virtual function_ptr_t fetch_function(const std::string_view & name) = 0;
};
} // namespace interop
