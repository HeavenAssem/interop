#pragma once
//
// Created by islam on 07.05.17.
//


#include "base_module.h"
#include "shared_library.h"

#include "module_metadata.h"



namespace mosaic {
    class module_context;
    class native_module_configuration_t;

    class native_module_t: public base_module_t {
        using base = base_module_t;

        shared_library  library;
        version_t       mosaic_abi_version;

    public:
        native_module_t(shared_library && library, const native_module_configuration_t &);
        native_module_t(const native_module_t &)  = delete;
        native_module_t(native_module_t &&) noexcept;

        void link(module_context & context) const override;

        void listen(const std::string_view & module_name, std::function<void()> && handler) override;

        const std::string& name() const override;

        ~native_module_t() noexcept;

        void unload() override;

    private:
        function_ptr_t create_function(const std::string & name) override;

        void validate_metadata() const;
    };
}
