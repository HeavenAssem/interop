//
// Created by islam on 10.04.18.
//

#pragma once

#include "internal_module.hpp"
#include "manager.hpp"

#include <v8.h>

namespace interop {
struct platform_module_configuration_t;

class platform_v8_module_t: public internal_module_t {
    using base = internal_module_t;

    v8::Isolate * isolate = nullptr;
    v8::UniquePersistent<v8::Context> persistent_context;
    class_manager_t class_manager;

  public:
    platform_v8_module_t(v8::Isolate * isolate, const platform_module_configuration_t &);
    ~platform_v8_module_t();

    void link(node_t & node) override;

    object_ptr_t create_dynamic(const std::string_view & name, arg_pack_t) override;
    void listen(const std::string_view & module_name, std::function<void()> && handler) override;
    function_ptr_t fetch_function(const std::string_view & name) override;
    void unload() override;

    v8::Isolate * get_isolate() const { return isolate; }
    v8::Local<v8::Context> get_context() const { return persistent_context.Get(isolate); }

    void initiate_garbage_collection_for_testing() const;

  private:
    void initialize() const;
    void expose_module(v8::Local<v8::Object> parent, v8::Local<v8::String> key, native_module_t &);
    void expose_module(v8::Local<v8::Object> parent, v8::Local<v8::String> key,
                       internal_module_t &);
};
} // namespace interop
