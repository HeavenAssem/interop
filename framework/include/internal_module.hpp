#pragma once
//
// Created by islam on 08.04.18.
//

#include "declarations.h"
#include "module_view.h"

namespace interop {

class internal_module_t: public internals::object_caching_layer_t<module_view_t> {

    using base_t = internals::object_caching_layer_t<module_view_t>;

    class_id_t next_class_id{unassigned_id_c, 0};

  public:
    using base_t::base_t;
    using base_t::get; // TODO: is it REALLY needed??

    void assign_id(module_id_t);
    module_id_t get_id() const;

    virtual void link(node_t &){};
    virtual void unload();
};
} // namespace interop
