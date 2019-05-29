#pragma once
//
// Created by islam on 07.05.17.
//

#include "internal_module.hpp"
#include "shared_library.h"

#include "module_metadata.h"

#include <lazy_sequence.h>

namespace interop {
class module_context_t;
struct native_module_configuration_t;

class native_module_t: public internal_module_t {
    using base = internal_module_t;

    shared_library_t library;
    version_t interop_abi_version;

    explicit native_module_t(shared_library_t);

  public:
    using classes_sequence_t   = lazy_sequence_t<std::pair<class_id_t, const object_metadata_t &>>;
    using functions_sequence_t = const std::vector<function_metadata_t> &;

    native_module_t(shared_library_t library, const native_module_configuration_t &);
    native_module_t(const native_module_t &) = delete;
    native_module_t(native_module_t &&) noexcept;

    classes_sequence_t iterate_classes() const;
    functions_sequence_t iterate_functions() const;
    const object_metadata_t & get_object_metadata(class_id_t) const;

    void link(node_t &) override;
    object_ptr_t create_dynamic(const std::string_view & name, arg_pack_t) override;

    void listen(const std::string_view & module_name, std::function<void()> && handler) override;

    ~native_module_t() noexcept;

    void unload() override;

  private:
    function_ptr_t fetch_function(const std::string_view & name) override;

    void validate_metadata() const;
};
} // namespace interop
