#pragma once
//
// Created by islam on 07.05.17.
//


#include "module_metadata.h"

#include <unordered_map>
#include <string>


namespace interop {
    class scope {
        std::unordered_map<std::string, module_metadata> modules_metadata;
    public:

    };
}
