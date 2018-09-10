//
// Created by islam on 27.05.17.
//

#pragma once

#include "type_metadata.h"

#include <vector>


namespace interop {
    struct constructor_metadata {
        void * pointer;
        std::vector<type_metadata> arguments;
    };

    struct destructor_metadata {
        void * pointer;
    };

    struct function_metadata {
        void * pointer = nullptr;
        std::string name;
        std::vector<type_metadata> arguments;
        type_metadata return_type;

        bool is_empty() const {
            return name.empty();
        }

        bool is_native() const {
            return !is_empty() && pointer != nullptr;
        }
    };
}
