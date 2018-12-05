#pragma once

#include "exceptions.h"
#include "function_metadata.h"
#include "type_subsystem/mappings.h"

namespace interop {
namespace internals {
using metadata_checker_t = void (*)(const function_metadata_t &);

template <class R, class... Args>
class strict_call_validator_t {
    template <int index, class Arg>
    static void check_args_types(const std::string & name,
                                 const std::vector<type_metadata_t> & metadata)
    {
        const auto expected_type   = metadata[index].type;
        constexpr auto passed_type = type_subsystem::enumerate_type<Arg>();
        if (passed_type != expected_type) {
            throw arguments_mismatch_error_t(
                "at function " + name +
                ": argument type mismatch when performing strict call on argument number " +
                std::to_string(index + 1) + ": expected - " + to_string(expected_type) +
                ", passed - " + to_string(passed_type));
        }
    }

    template <int index, class Arg1, class Arg2, class... _Args>
    static void check_args_types(const std::string & name,
                                 const std::vector<type_metadata_t> & metadata)
    {
        check_args_types<index, Arg1>(name, metadata);
        check_args_types<index + 1, Arg2, _Args...>(name, metadata);
    }

    template <int>
    static void check_args_types(const std::string &, const std::vector<type_metadata_t> &)
    {}

  public:
    static void check_args(const std::string & name, const std::vector<type_metadata_t> & metadata)
    {
        constexpr auto argument_count = sizeof...(Args);
        if (argument_count != metadata.size()) {
            throw arguments_mismatch_error_t(
                "at function " + name + ": argument count mismatch: expected: " +
                std::to_string(metadata.size()) + "; got: " + std::to_string(argument_count));
        }
        check_args_types<0, Args...>(name, metadata);
    }

    static void check_return_type(const std::string & name, const type_metadata_t & metadata)
    {
        if constexpr (!std::is_void<R>::value) { // Allow to discard return value (when no return
                                                 // type specified on call - do not perform check).
            const auto & expected_type = metadata.type;
            const auto & passed_type   = type_subsystem::enumerate_type<R>();
            if (passed_type != expected_type) {
                throw function_call_error_t(
                    "at function " + name +
                    ": return type mismatch when performing strict call : expected - " +
                    to_string(expected_type) + ", passed - " + to_string(passed_type));
            }
        }
    }

    strict_call_validator_t(const function_metadata_t & metadata,
                            std::vector<metadata_checker_t> & checkers)
    {
        checkers.push_back(&strict_call_validator_t::check_metadata);
        check_metadata(metadata);
    }

    static void check_metadata(const function_metadata_t & metadata)
    {
        if (metadata.is_native()) {
            check_args(metadata.name, metadata.arguments);
            check_return_type(metadata.name, metadata.return_type);
        }
    }
};
} // namespace internals
} // namespace interop
