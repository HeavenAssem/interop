//
// Created by islam on 13.05.17.
//

#pragma once

#include "function_view.h"

#include "definitions.h"

#include <functional>


namespace mosaic {
    class module_view {
    public:
        virtual function_ptr function(const std::string_view& name) const = 0;
        virtual void listen(const std::string_view & module_name, const std::function<void()> handler) = 0;
        virtual const std::string& name() const = 0;
    };
}
