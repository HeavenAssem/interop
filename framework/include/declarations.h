//
// Created by islam on 24.12.17.
//

#pragma once

#include <memory>

namespace mosaic {
    class platform;
    class platform_factory;

    using platform_ptr = std::shared_ptr<platform>;
    using platform_factory_ptr = std::shared_ptr<platform_factory>;
}
