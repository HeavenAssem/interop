#pragma once

#include <function_view.hpp>

namespace interop {
class native_function_t final: public function_view_t {
    universal_wrapper_t dynamic_invoke;

  public:
    explicit native_function_t(const function_metadata_t &, void * object = nullptr);
    explicit native_function_t(const constructor_metadata_t &, std::string object_name);

    val_t call_dynamic(arg_pack_t = {}) const override;

    static val_t call_dynamic(universal_wrapper_t, void * context, arg_pack_t = {},
                              const std::vector<type_metadata_t> & = {});
};
} // namespace interop
