#pragma once
//
// Created by islam on 24.12.17.
//

#include "declarations.h"

#include <type_traits.hpp>

#include <string_view>

namespace interop {
class platform_factory_t {
  public:
    virtual platform_ptr instantiate() = 0;
    virtual ~platform_factory_t()      = default;
};

template <class Platform>
class wrapped_platform final: public platform_factory_t {
    static_assert(is_derived_from<platform_t, Platform>::value,
                  "platform must be derived from interop::platform");

  public:
    platform_ptr instantiate() override { return std::make_shared<Platform>(); }
};

template <class Platform>
platform_factory_ptr wrap_platform()
{
    return std::make_shared<wrapped_platform<Platform>>();
}

struct platform_registrator {
    platform_registrator(const std::string & id, platform_factory_ptr platform);
};

platform_ptr instantiate_platform(const std::string & id);
} // namespace interop

#define register_platform_as(ID, PLATFORM)                                                         \
    static platform_registrator __registrator_object__(ID, wrap_platform<PLATFORM>());
