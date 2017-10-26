//
// Created by islam on 06.05.17.
//

#pragma once

#include "definitions.h"

#include <exception>
#include <string>


namespace mosaic {
    class error: std::exception {
        std::string message;
    public:
        explicit error(const std::string_view& what);
        error(const error &) = default;

        const char * what() const noexcept override;

        void append(const std::string_view& message);
    };

    class function_call_error: public error {
    public:
        explicit function_call_error(const std::string_view& what);
    };

    class lookup_error: public error {
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

    class object_lookup_error: public lookup_error {
    public:
        explicit object_lookup_error(const std::string_view& what);
    };

    class not_implemented: public error {
    public:
        explicit not_implemented(const std::string_view& what);
    };

    class open_error: public error {
    public:
        explicit open_error(const std::string_view& what);
    };

    class validation_error: public error {
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

    class loading_error: public error {
    public:
        explicit loading_error(const std::string_view& what);
    };

    class node_loading_error: public loading_error {
    public:
        explicit node_loading_error(const std::string_view & what);
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
}
