#pragma once

#include "strict_call_validator.h"

#include <invariant.h>

#include <algorithm>
#include <string_view>

namespace interop {
namespace internals {

template <typename Metadata>
auto * find_metadata_opt(Metadata & metadata, std::string_view name)
{
    auto it = std::find_if(metadata.begin(), metadata.end(),
                           [&](auto & inner_metadata) { return name == inner_metadata.name; });
    if (it == metadata.end()) {
        return decltype(&*it){nullptr};
    }

    return &*it;
}

template <typename Metadata, typename Handler>
auto & find_metadata(Metadata & metadata, std::string_view name, Handler && not_found)
{
    if (auto ptr = find_metadata_opt(metadata, name)) {
        return *ptr;
    } else {
        not_found();
        interop_invariant_m(false, "not_found handler didn't throw");
    }
}

template <typename... Args, typename Metadata, typename Handler>
auto & find_first_with_args(Metadata & metadata, Handler && not_found)
{
    auto it = std::find_if(metadata.begin(), metadata.end(), [](auto & inner_metadata) {
        try {
            strict_call_validator_t<void, Args...>::check_args("", inner_metadata.arguments);
        } catch (const arguments_mismatch_error_t &) {
            return false;
        }
        return true;
    });

    if (it == metadata.end()) {
        not_found();
        interop_invariant_m(false, "not_found handler didn't throw");
    }

    return *it;
}

} // namespace internals
} // namespace interop
