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
    using error_t::error_t;
};

class function_call_error_t: public error_t {
    using error_t::error_t;
};

class lookup_error_t: public error_t {
    using error_t::error_t;
};

class module_lookup_error_t: public lookup_error_t {
    using lookup_error_t::lookup_error_t;
};

class function_lookup_error_t: public lookup_error_t {
    using lookup_error_t::lookup_error_t;
};

class type_lookup_error_t: public lookup_error_t {
    using lookup_error_t::lookup_error_t;
};

class not_implemented: public error_t {
    using error_t::error_t;
};

class open_error_t: public error_t {
    using error_t::error_t;
};

class validation_error_t: public error_t {
    using error_t::error_t;
};

class module_validation_error_t: public validation_error_t {
    using validation_error_t::validation_error_t;
};

class open_path_error_t: public open_error_t {
    using open_error_t::open_error_t;
};

class open_file_error_t: public open_error_t {
    using open_error_t::open_error_t;
};

class register_error_t: public error_t {
    using error_t::error_t;
};

class loading_error_t: public error_t {
    using error_t::error_t;
};

class node_loading_error_t: public loading_error_t {
    using loading_error_t::loading_error_t;
};

class module_loading_error_t: public loading_error_t {
    using loading_error_t::loading_error_t;
};

class library_loading_error_t: public loading_error_t {
    using loading_error_t::loading_error_t;
};

class symbol_loading_error_t: public loading_error_t {
    using loading_error_t::loading_error_t;
};

class library_unloading_error_t: public loading_error_t {
    using loading_error_t::loading_error_t;
};

class platform_loading_error_t: public loading_error_t {
    using loading_error_t::loading_error_t;
};

class typing_error_t: public error_t {
    using error_t::error_t;
};

class type_mismatch_error_t: public typing_error_t {
    using typing_error_t::typing_error_t;
};
} // namespace interop
