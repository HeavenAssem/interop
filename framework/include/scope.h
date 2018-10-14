#pragma once
//
// Created by islam on 07.05.17.
//

#include "module_metadata.h"

#include <string>
#include <unordered_map>

namespace interop {
class scope {
    std::unordered_map<std::string, module_metadata_t> modules_metadata;

  public:
};
} // namespace interop
