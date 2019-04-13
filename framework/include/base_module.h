#pragma once
//
// Created by islam on 08.04.18.
//

#include "caches.hpp"
#include "module_view.h"

#include <lazy_sequence.h>

namespace interop {
class node_t;

class base_module_t: public module_view_t {
    mutable function_cache_t cache;

  protected:
    module_metadata_t metadata;
    module_id_t id = unassigned_id_c;

  public:
    using classes_sequence_t = lazy_sequence_t<std::pair<class_id_t, const object_metadata_t &>>;

    base_module_t();
    base_module_t(base_module_t &&) = default;

    void assign_id(module_id_t);
    module_id_t get_id() const;

    virtual void link(node_t &){};

    classes_sequence_t iterate_classes() const;
    const object_metadata_t & get_object_metadata(class_id_t) const;

    const module_metadata_t & get_metadata() const override final;
    function_ptr_t function(const std::string_view & name) override final;

    virtual void unload();

  protected:
    virtual function_ptr_t fetch_function(const std::string_view & name) = 0;
};
} // namespace interop
