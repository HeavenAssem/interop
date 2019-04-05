#pragma once

#include <declarations.h>

#include <v8.h>

#include <string_view>
#include <unordered_map>

namespace interop {

class class_manager_t {
  public:
    class object_manager_t {
      public:
        struct object_context_t {
            object_view_t & object;
            v8::Persistent<v8::Object> weak_ref;
        };

      private:
        std::string_view class_name;
        std::unordered_map<object_view_t *, object_context_t> managed_objects;

      public:
        object_manager_t(std::string_view);
        const std::string_view & get_class_name() const { return class_name; }
    };

  private:
    std::unordered_map<std::string, object_manager_t> managed_classes;

  public:
};
} // namespace interop
