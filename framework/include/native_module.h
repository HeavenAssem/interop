#pragma once
//
// Created by islam on 07.05.17.
//

#include "base_module.h"
#include "shared_library.h"

#include "module_metadata.h"

namespace interop {
class module_context_t;
struct native_module_configuration_t;

class native_module_t: public base_module_t {
    using base = base_module_t;

    version_t interop_abi_version;
    shared_library library;

  public:
    native_module_t(shared_library && library, const native_module_configuration_t &);
    native_module_t(const native_module_t &) = delete;
    native_module_t(native_module_t &&) noexcept;

    void link(node_t &) override;
    object_ptr_t create_dynamic(const std::string_view & name, arg_pack_t) const override;

    void listen(const std::string_view & module_name, std::function<void()> && handler) override;

    const std::string & name() const override;

    ~native_module_t() noexcept;

    void unload() override;

  private:
    function_ptr_t fetch_function(const std::string_view & name) override;

    void validate_metadata() const;
};
} // namespace interop
