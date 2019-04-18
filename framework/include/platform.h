//
// Created by islam on 07.11.17.
//

#pragma once

#include "declarations.h"

#include <function_view.hpp>
#include <object_view.hpp>

#include <string>

namespace interop {
class platform_function_t: public function_view_t {
  public:
    platform_function_t(std::string name)
      : function_view_t{std::move(name)}
    {}
};

class platform_object_t: public object_view_t {
  protected:
    std::string object_name;

  public:
    platform_object_t(std::string name)
      : object_name{std::move(name)}
    {}

    const std::string & name() const override { return object_name; }
};

class platform_t {
  public:
    virtual ~platform_t() = default;

    virtual std::vector<module_ptr_t> initialize(const platform_configuration_t &) = 0;
    virtual void run()                                                             = 0;
    virtual void dispose()                                                         = 0;
};
} // namespace interop
