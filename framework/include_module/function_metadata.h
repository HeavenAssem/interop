//
// Created by islam on 27.05.17.
//

#pragma once

#include "type_metadata.h"

#include <vector>


namespace mosaic {
    struct constructor_metadata {
        void * pointer;
        std::vector<type_metadata> arguments;
    };

    struct destructor_metadata {
        void * pointer;
    };

    struct function_metadata {
        void * pointer;
        std::string name;
        std::vector<type_metadata> arguments;
        type_metadata return_type;
    };
}
