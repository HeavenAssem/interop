//
// Created by islam on 26.10.17.
//

#pragma once

#include <memory>

namespace mosaic {
    class object_view;
    class function_view;

    using object_ptr = std::shared_ptr<object_view>;
    using function_ptr = std::shared_ptr<function_view>;
}
