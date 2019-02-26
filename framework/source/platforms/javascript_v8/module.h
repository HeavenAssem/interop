//
// Created by islam on 10.04.18.
//

#pragma once

#include "base_module.h"

#include <v8.h>

namespace interop {
struct platform_module_configuration_t;

class platform_v8_module_t: public base_module_t {
    using base = base_module_t;

    v8::Isolate * isolate = nullptr;
    v8::UniquePersistent<v8::Context> context;

  public:
    platform_v8_module_t(v8::Isolate * isolate, const platform_module_configuration_t &);
    ~platform_v8_module_t();

    void link(node_t & node) const override;

    void listen(const std::string_view & module_name, std::function<void()> && handler) override;
    function_ptr_t fetch_function(const std::string_view & name) override;
    const std::string & name() const override;
    void unload() override;

    v8::Isolate * get_isolate();
    v8::UniquePersistent<v8::Context> & get_context();
};
} // namespace interop
