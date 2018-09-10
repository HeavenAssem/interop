//
// Created by islam on 14.01.18.
//

#pragma once

#include "conventions.h"

#include <string>

namespace interop {
    struct version_t {
        uint16_t major;
        uint16_t minor;
        uint16_t patch;
        uint16_t tweak;

        constexpr uint64_t value() const noexcept {
            return (uint64_t(major) << 48) + (uint64_t(minor) << 32) + (uint32_t(patch) << 16) + tweak;
        }

        std::string to_string() const {
            return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch) + "." + std::to_string(tweak);
        }

        constexpr bool operator==(const version_t & rhs) const noexcept {
            return value() == rhs.value();
        }

        constexpr bool operator!=(const version_t & rhs) const noexcept {
            return value() != rhs.value();
        }

        constexpr bool operator>(const version_t & rhs) const noexcept {
            return value() > rhs.value();
        }
    };

    extern "C" {
        extern const version_t MOSAIC_FRAMEWORK_ABI_VERSION;
    }
}

#define MOSAIC_DEFINE_CURRENT_ABI_VERSION const version_t MOSAIC_FRAMEWORK_ABI_VERSION { 0, 1, 0, 0 };
