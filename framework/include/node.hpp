#pragma once
//
// Created by islam on 07.05.17.
//

#include "declarations.h"
#include "module_context.h"
#include "node_metadata.hpp"

#include <definitions.h>
#include <lazy_sequence.h>

#include <memory>
#include <unordered_map>

namespace interop {
struct node_configuration_t;
struct native_module_configuration_t;

class node_t: public module_context_t {
    // static std::unordered_map<std::string, std::shared_ptr<native_module_t>> global_scope;

    node_metadata_t metadata;

    std::string name;
    std::vector<module_ptr_t> modules;
    std::vector<platform_ptr_t> platforms;
    std::unordered_map<std::string, module_ptr_t::pointer> modules_by_name;
    std::unordered_map<std::string, platform_ptr_t::pointer> platforms_by_name;

    mutable module_id_t next_module_id = {};

    node_t(const node_configuration_t &);
    node_t(const node_t &) = delete;
    node_t(node_t &&)      = delete;

    template <typename ModuleType>
    static lazy_sequence_value_t<ModuleType &> only_castable_to(internal_module_t & module)
    {
        if (auto casted_module_ptr = dynamic_cast<ModuleType *>(&module)) {
            return *casted_module_ptr;
        }
        return std::nullopt;
    }

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

    // not thread_safe
    module_id_t get_new_module_id() const { return next_module_id++; }

    /**
     * @brief get module by name
     *
     * @param name
     * @return module_view_t&
     */
    module_view_t & get(const std::string & name) override;
    /** public interface end **/

    /** internal interface **/
    using modules_sequence_t = lazy_sequence_t<internal_module_t &>;

    void link();

    modules_sequence_t iterate_modules();
    template <typename ModuleType>
    auto iterate()
    {
        static_assert(std::is_base_of_v<module_view_t, ModuleType>, "wrong type to iterate");
        return iterate_modules().transformed(only_castable_to<ModuleType>);
    }

    internal_module_t & get_module(module_id_t);
    template <typename ModuleType>
    ModuleType & get(module_id_t module_id)
    {
        auto & module = get_module(module_id);
        if (auto casted_module_ptr = dynamic_cast<ModuleType *>(&module)) {
            return *casted_module_ptr;
        }

        throw error_t("no module with requested type and id");
    }

    inline const node_metadata_t & get_metadata() const { return metadata; }

    void unload(bool forced = false);
    /** internal interface **/

    virtual ~node_t();

  private:
    void load_native_module(const native_module_configuration_t &);

    void forced_unload() noexcept;
};
} // namespace interop
