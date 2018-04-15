//
// Created by islam on 20.06.17.
//

#pragma once

#include <memory>
#include "function_metadata.h"


namespace mosaic {
    struct object_metadata {
        std::string                         name;
        destructor_metadata                 destructor;
        std::vector<constructor_metadata>   constructors;
        std::vector<variable_metadata>      fields;
        std::vector<function_metadata>      methods;
    };
}