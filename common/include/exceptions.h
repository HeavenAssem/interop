//
// Created by islam on 06.05.17.
//

#pragma once

#include "definitions.h"

#include <exception>
#include <string>
#include <string_view>

namespace interop {
class error_t: std::exception {
    std::string message;

  public:
    explicit error_t(std::string what);
    error_t(const error_t &) = default;

    const char * what() const noexcept override;

    void append(const std::string_view & message);
};

class arguments_mismatch_error_t: public error_t {
  public:
    explicit arguments_mismatch_error_t(std::string what);
};

class function_call_error_t: public error_t {
  public:
    explicit function_call_error_t(std::string what);
};

class lookup_error_t: public error_t {
  public:
    explicit lookup_error_t(std::string what);
};

class module_lookup_error_t: public lookup_error_t {
  public:
    explicit module_lookup_error_t(std::string what);
};

class function_lookup_error_t: public lookup_error_t {
  public:
    explicit function_lookup_error_t(std::string what);
};

class type_lookup_error_t: public lookup_error_t {
  public:
    explicit type_lookup_error_t(std::string what);
};

class not_implemented: public error_t {
  public:
    explicit not_implemented(std::string what);
};

class open_error_t: public error_t {
  public:
    explicit open_error_t(std::string what);
};

class validation_error_t: public error_t {
  public:
    explicit validation_error_t(std::string what);
};

class module_validation_error_t: public validation_error_t {
  public:
    explicit module_validation_error_t(std::string what);
};

class open_path_error_t: public open_error_t {
  public:
    explicit open_path_error_t(std::string what);
};

class open_file_error_t: public open_error_t {
  public:
    explicit open_file_error_t(std::string what);
};

class register_error_t: public error_t {
  public:
    explicit register_error_t(std::string what);
};

class loading_error_t: public error_t {
  public:
    explicit loading_error_t(std::string what);
};

class node_loading_error_t: public loading_error_t {
  public:
    explicit node_loading_error_t(std::string what);
};

class module_loading_error_t: public loading_error_t {
  public:
    explicit module_loading_error_t(std::string what);
};

class library_loading_error_t: public loading_error_t {
  public:
    explicit library_loading_error_t(std::string what);
};

class symbol_loading_error_t: public loading_error_t {
  public:
    explicit symbol_loading_error_t(std::string what);
};

class library_unloading_error_t: public loading_error_t {
  public:
    explicit library_unloading_error_t(std::string what);
};

class platform_loading_error_t: public loading_error_t {
    using loading_error_t::loading_error_t;
};

class type_mismatch_error_t: public error_t {
    using error_t::error_t;
};
} // namespace interop
