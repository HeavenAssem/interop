//
// Created by islam on 07.05.17.
//

#pragma once

#include "module_metadata.h"
#include "module_context.h"


namespace mosaic {
    typedef module_metadata (*describe_module_function)();
    typedef void (*initialize_module_function)(module_context &);
    typedef void (*register_module_function)();
    typedef void (*shutdown_module_function)();
}

