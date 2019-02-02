//
// Created by islam on 27.05.17.
//

#pragma once

#include "declarations.h"
#include "type_metadata.h"

#include <vector>

namespace interop {
struct constructor_metadata_t {
    void * pointer;
    std::vector<type_metadata_t> arguments;
};

struct destructor_metadata_t {
    void * pointer;
};

typedef val_t (*universal_wrapper_t)(void *, arg_pack_t);

struct function_metadata_t {
    void * pointer             = nullptr;
    void * context             = nullptr;
    universal_wrapper_t invoke = nullptr;
    std::string name;
    std::vector<type_metadata_t> arguments;
    type_metadata_t return_type;

    bool is_empty() const { return name.empty(); }
    bool is_native() const { return !is_empty() && pointer != nullptr; }
};
} // namespace interop
