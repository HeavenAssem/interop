//
// Created by islam on 07.05.17.
//

#pragma once

#include "type_metadata.h"
#include "object_metadata.h"
#include "version.h"

#include <vector>

namespace mosaic {
    struct module_metadata {
        std::string name;
        version_t version;
        std::vector<function_metadata> functions;
        std::vector<object_metadata> types;
    };
}
