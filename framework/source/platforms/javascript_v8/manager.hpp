#pragma once

#include <declarations.h>

#include <v8.h>

#include <unordered_map>

namespace interop {

class class_manager_t {
  public:
    class object_manager_t {
        friend class_manager_t;

        struct object_context_t {
            object_ptr_t object;
            v8::Persistent<v8::Object> weak_ref;

            object_context_t(object_ptr_t, v8::Local<v8::Object> bind_to);
        };

        class_id_t class_id;
        std::unordered_map<object_view_t *, object_context_t> managed_objects;

      public:
        object_manager_t(class_id_t);
        native_module_t & get_module() const;
        const std::string & get_class_name() const;

        void manage_lifetime(object_ptr_t, v8::Local<v8::Object> bind_to);

      private:
        static void weak_callback(const v8::WeakCallbackInfo<void> &);
    };

  private:
    std::unordered_map<class_id_t, object_manager_t, class_id_hash_t> managed_classes;

  public:
    class_manager_t()                        = default;
    class_manager_t(const class_manager_t &) = delete;
    class_manager_t(class_manager_t &&)      = delete;

    class_manager_t & operator=(const class_manager_t &) = delete;
    class_manager_t & operator=(class_manager_t &&) = delete;

    object_manager_t & get_manager_for_class(class_id_t);
};

using object_manager_t = class_manager_t::object_manager_t;

} // namespace interop
