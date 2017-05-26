//
// Created by islam on 07.05.17.
//

#pragma once

#include <string>
#include <vector>

namespace mosaic {
    struct type_metadata {
        std::string name;
        std::size_t size;
    };

    struct function_metadata {
        void * pointer;
        std::string name;
        std::vector<type_metadata> arguments;
        type_metadata return_type;
    };

    struct module_version {
        uint16_t major;
        uint16_t minor;
        uint16_t patch;
        uint16_t tweak;
        uint64_t value() {
            return (uint64_t(major) << 48) + (uint64_t(minor) << 32) + (uint32_t(patch) << 16) + tweak;
        }
    };

    struct module_metadata {
        std::string name;
        module_version version;
        std::vector<function_metadata> functions;
    };

}
