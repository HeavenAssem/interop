//
// Created by islam on 24.12.17.
//

#pragma once

#include <memory>
#include <vector>
#include <any>

namespace mosaic {
    class module_view;
    class base_module_t;

    class platform_t;
    class platform_factory_t;
    class platform_function_t;

    struct platform_configuration_t;

    using platform_ptr          = std::shared_ptr<platform_t>;
    using platform_factory_ptr  = std::shared_ptr<platform_factory_t>;
    using platform_function_ptr = std::shared_ptr<platform_function_t>;

    using val_t       = std::any;
    using arg_pack_t  = std::vector<val_t>;
}
