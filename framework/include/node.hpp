#pragma once
//
// Created by islam on 07.05.17.
//

#include "declarations.h"
#include "module_context.h"

#include <definitions.h>
#include <lazy_sequence.h>

#include <memory>
#include <unordered_map>

namespace interop {
struct node_configuration_t;
struct native_module_configuration_t;

class node_t: public module_context_t {
    // static std::unordered_map<std::string, std::shared_ptr<native_module_t>> global_scope;

    std::string name;
    std::vector<module_ptr_t> modules;
    std::vector<platform_ptr_t> platforms;
    std::unordered_map<std::string, module_ptr_t::pointer> modules_by_name;
    std::unordered_map<std::string, platform_ptr_t::pointer> platforms_by_name;

    node_t(const node_configuration_t &);
    node_t(const node_t &) = delete;
    node_t(node_t &&)      = delete;

  public:
    /** public interface **/

    /**
     * @brief create and configure singleton node instance
     *
     * @details not thread-safe
     *
     * @throws interop::error_t if called after successful return
     *
     * @return node_t&
     */
    static node_t & start(const node_configuration_t &);

    /**
     * @brief get current node instance
     *
     * @throws interop::error_t if called before successful start
     *
     * @return node_t&
     */
    static node_t & get();

    /**
     * @brief get module by name
     *
     * @param name
     * @return module_view_t&
     */
    module_view_t & get(const std::string & name) override;
    /** public interface end **/

    /** internal interface **/
    using modules_sequence_t = lazy_sequence_t<std::pair<module_id_t, base_module_t &>>;

    void link();
    modules_sequence_t iterate_modules();
    base_module_t & get(module_id_t);
    void unload(bool forced = false);
    /** internal interface **/

    virtual ~node_t();

  private:
    void load_native_module(const native_module_configuration_t &);

    void forced_unload() noexcept;
};
} // namespace interop
