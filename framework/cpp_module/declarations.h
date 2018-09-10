//
// Created by islam on 26.10.17.
//

#pragma once

#include <memory>

namespace interop {
    class object_view;
    class function_view;
    class platform_function_t;

    using object_ptr            = std::shared_ptr<object_view>;
    using function_ptr_t        = std::shared_ptr<function_view>;
    using platform_function_ptr = std::shared_ptr<platform_function_t>;
}
