#pragma once
//
// Created by islam on 24.12.17.
//

#include "declarations.h"

#include <string_view>

namespace mosaic {
    class platform_factory_t {
    public:
        virtual platform_ptr instantiate() = 0;
        virtual ~platform_factory_t() = default;
    };

    template <class Platform>
    class wrapped_platform final: public platform_factory_t {
        static_assert(std::is_base_of<platform_t, Platform>::value, "platform must be derived from mosaic::platform");
    public:
        platform_ptr instantiate() override {
            return std::make_shared<Platform>();
        }
    };

    template <class Platform>
    platform_factory_ptr wrap_platform() {
        return std::make_shared<wrapped_platform<Platform>>();
    }

    struct platform_registrator {
        platform_registrator(const std::string & id, platform_factory_ptr platform);
    };

    platform_ptr instantiate_platform(const std::string & id);
}

#define register_platform_as(_id, _platform) static platform_registrator __registrator_object__(_id, wrap_platform<_platform>());
