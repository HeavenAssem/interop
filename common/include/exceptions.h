//
// Created by islam on 06.05.17.
//

#pragma once

#include "definitions.h"

#include <exception>
#include <string>


namespace interop {
    class error_t: std::exception {
        std::string message;
    public:
        explicit error_t(const std::string_view& what);
        error_t(const error_t &) = default;

        const char * what() const noexcept override;

        void append(const std::string_view& message);
    };

    class arguments_mismatch_error: public error_t {
    public:
        explicit arguments_mismatch_error(const std::string_view& what);
    };

    class function_call_error: public error_t {
    public:
        explicit function_call_error(const std::string_view& what);
    };

    class lookup_error: public error_t {
    public:
        explicit lookup_error(const std::string_view& what);
    };

    class module_lookup_error: public lookup_error {
    public:
        explicit module_lookup_error(const std::string_view& what);
    };

    class function_lookup_error: public lookup_error {
    public:
        explicit function_lookup_error(const std::string_view& what);
    };

    class type_lookup_error: public lookup_error {
    public:
        explicit type_lookup_error(const std::string_view& what);
    };

    class not_implemented: public error_t {
    public:
        explicit not_implemented(const std::string_view& what);
    };

    class open_error: public error_t {
    public:
        explicit open_error(const std::string_view& what);
    };

    class validation_error: public error_t {
    public:
        explicit validation_error(const std::string_view& what);
    };

    class module_validation_error: public validation_error {
    public:
        explicit module_validation_error(const std::string_view& what);
    };

    class open_path_error: public open_error {
    public:
        explicit open_path_error(const std::string_view& what);
    };

    class open_file_error: public open_error {
    public:
        explicit open_file_error(const std::string_view& what);
    };

    class register_error: public error_t {
    public:
        explicit register_error(const std::string_view& what);
    };

    class loading_error: public error_t {
    public:
        explicit loading_error(const std::string_view& what);
    };

    class node_loading_error: public loading_error {
    public:
        explicit node_loading_error(const std::string_view & what);
    };

    class module_loading_error: public loading_error {
    public:
        explicit module_loading_error(const std::string_view & what);
    };

    class library_loading_error: public loading_error {
    public:
        explicit library_loading_error(const std::string_view& what);
    };

    class symbol_loading_error: public loading_error {
    public:
        explicit symbol_loading_error(const std::string_view& what);
    };

    class library_unloading_error: public loading_error {
    public:
        explicit library_unloading_error(const std::string_view& what);
    };

    class platform_loading_error: public loading_error {
    public:
        explicit platform_loading_error(const std::string_view& what);
    };
}
