#pragma once
//
// Created by islam on 13.05.17.
//


#include "module_view.h"

#include <declarations.h>


namespace interop {
    class module_context {
    public:
        virtual module_view & get(const std::string & name) = 0;
        virtual ~module_context() = default;
    };
}
