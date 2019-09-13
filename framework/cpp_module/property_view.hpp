#pragma once

#include <string>

namespace interop {
struct property_view_t {
    ~property_view_t() = default;

    virtual const std::string & name() const = 0;
};
} // namespace interop
