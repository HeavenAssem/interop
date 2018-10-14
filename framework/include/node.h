#pragma once
//
// Created by islam on 07.05.17.
//

#include "declarations.h"
#include "module_context.h"

#include <memory>
#include <unordered_map>

#include <definitions.h>

namespace interop {
struct node_configuration_t;
struct native_module_configuration_t;

enum iteration_status_e : bool { Done, Continue };

class node_t: public module_context_t {
    // static std::unordered_map<std::string, std::shared_ptr<native_module_t>> global_scope;

    std::string name;
    std::unordered_map<std::string, module_ptr> local_scope;
    std::unordered_map<std::string, platform_ptr> platforms;

  public:
    node_t(const node_configuration_t &);
    node_t(const node_t &) = delete;
    node_t(node_t &&);

    void link();
    void for_each_module(std::function<iteration_status_e(const module_ptr &)>);

    module_view_t & get(const std::string & name) override;

    virtual ~node_t();

    void unload(bool forced = false);

  private:
    void load_native_module(const native_module_configuration_t &);

    void forced_unload() noexcept;
};
} // namespace interop
