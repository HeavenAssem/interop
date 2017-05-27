//
// Created by islam on 27.05.17.
//

#pragma once

#include "type_metadata.h"

#include <vector>


namespace mosaic {
    struct function_metadata {
        void * pointer;
        std::string name;
        std::vector<type_metadata> arguments;
        type_metadata return_type;
    };
}
