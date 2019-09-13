#pragma once

#include <map>
#include <stdint.h>
#include <typeindex>
#include <vector>

namespace interop {

using user_type_index_t = uint16_t;

class user_type_registry_t {

    static inline std::vector<std::type_index> types;

    static user_type_index_t register_user_type(const std::type_info & info)
    {
        types.emplace_back(info);
        return types.size() - 1;
    }

    template <typename T>
    struct user_type_wrapper_t {
        // TODO: check
        static inline user_type_index_t index = register_user_type(typeid(T));
    };

  public:
    template <typename T>
    static void set()
    {
        std::ignore = user_type_registry_t::user_type_wrapper_t<T>::index;
    }

    template <typename T>
    static user_type_index_t get()
    {
        return user_type_wrapper_t<T>::index;
    }
};

} // namespace interop
