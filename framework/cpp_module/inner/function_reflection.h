#pragma once

#include "function_metadata.h"
#include "inner/common.h"

#include <functional>

/**
 * @brief represent any c++ callable object as c function
 */

namespace interop {
namespace internals {
namespace function_reflection {

template <typename ... Args>
struct arguments_reflector_t {
    static std::vector<type_metadata_t> arguments() {
        std::vector<type_metadata_t> arguments_metadata;
        meta::describe_types<Args...>(arguments_metadata);
        return std::move(arguments_metadata);
    }
};

template <typename R, typename ... Args>
struct signature_reflector_t: arguments_reflector_t<Args...> {
    static type_metadata_t return_type() {
        return meta::describe_type<R>();
    }
};

template <typename Callable> class function_reflector_t;

template <typename R, typename ... Args>
class function_reflector_t<R (*)(Args...)> {
    typedef R(*c_function_t)(Args...);
    using reflected_t = signature_reflector_t<R, Args...>;

public:
    static function_metadata reflect(std::string name, c_function_t c_function) {
        return {
            reinterpret_cast<void*>(c_function),
            nullptr,
            std::move(name),
            reflected_t::arguments(),
            reflected_t::return_type(),
        };
    }
};

struct functor_reflector_t {
    template <typename R, typename ... Args>
    static function_metadata reflect(std::string name, std::function<R(Args ...)> functor) {
        typedef R(*c_function_t)(void*, Args ...);
        using cpp_functor_t = decltype(functor);
        using reflected_t = signature_reflector_t<R, Args ...>;
        static std::unordered_map<std::string, cpp_functor_t> storage;

        auto insertion_result = storage.emplace(name, std::move(functor));

        if (!insertion_result.second) {
            throw register_error("name collision: " + name);
        }

        c_function_t proxy = [](void * context, Args ... args) -> R {
            auto ctx = reinterpret_cast<cpp_functor_t*>(context);
            return (*ctx)(std::forward<Args>(args)...);
        };

        return {
            reinterpret_cast<void*>(proxy),
            &*insertion_result.first,
            std::move(name),
            reflected_t::arguments(),
            reflected_t::return_type(),
        };
    }
};

}
}
}
