#pragma once
//
// Created by islam on 08.04.18.
//

#include "caches.hpp"
#include "declarations.h"
#include "module_view.h"

namespace interop {

class internal_module_t: public module_view_t {
    mutable function_cache_t cache;

    class_id_t next_class_id{unassigned_id_c, 0};

  public:
    using module_view_t::module_view_t;

    void assign_id(module_id_t);
    module_id_t get_id() const;

    function_ptr_t function(const std::string_view & name) override final;

    virtual void link(node_t &){};
    virtual void unload();

  protected:
    virtual function_ptr_t fetch_function(const std::string_view & name) = 0;
};
} // namespace interop
