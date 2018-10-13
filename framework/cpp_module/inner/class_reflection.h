#pragma once

#include "inner/function_reflection.h"

namespace interop {
namespace internals {
namespace class_reflection {

    template <class C>
    struct destructor_proxy_t {
        static void call(C * object) {
            return delete object;
        }
    };

    template <typename Class, typename ...Args>
    struct constructor_proxy_t {
        static Class * call(Args && ... args) {
            return new Class(std::forward<Args>(args)...);
        }
    };

    template <class ... Args>
    struct constructor_t {
        template <class Class>
        using proxy_t = constructor_proxy_t<Class, Args...>;
        using reflector_t = function_reflection::arguments_reflector_t<Args...>;
    };

    template <class Class>
    struct constructor_reflector_t {
        template <class Constructor>
        static void reflect(std::vector<constructor_metadata> & metadata) {
            using proxy_t = typename Constructor::template proxy_t<Class>;
            using reflector_t = typename Constructor::reflector_t;

            metadata.push_back(constructor_metadata{
                reinterpret_cast<void*>(proxy_t::call),
                reflector_t::arguments()
            });
        }
    };

    template<class Class, class ... Constructors>
    void register_constructors(object_metadata & metadata) {
        ((void) constructor_reflector_t<Class>::template reflect<Constructors>(metadata.constructors), ...);
    }

    template <class Class>
    void register_destructor(object_metadata & metadata) {
        metadata.destructor.pointer = (void *)destructor_proxy_t<Class>::call;
    }

    template <typename cpp_method_t, cpp_method_t> class method_reflector_t;

    template <typename R, typename C, typename ... Args, R (C::*cpp_method_ptr)(Args...)>
    class method_reflector_t<R (C::*)(Args...), cpp_method_ptr> {
        using reflected_t = function_reflection::signature_reflector_t<R, Args...>;
        typedef R(C::*cpp_method_t)(Args...);

        static R c_function(void * object, Args && ... args) {
            auto obj = reinterpret_cast<C*>(object);
            return (obj->*cpp_method_ptr)(std::forward<Args>(args)...);
        }

    public:
        using class_t = C;

        static function_metadata reflect(std::string name) {
            return {
                reinterpret_cast<void*>(c_function),
                nullptr,
                std::move(name),
                reflected_t::arguments(),
                reflected_t::return_type(),
            };
        }
    };

    template <class Class, typename ... Constructors>
    class class_reflector_t {
        object_metadata & metadata;

    public:
        class_reflector_t(object_metadata & metadata): metadata(metadata) {
            register_destructor<Class>(metadata);
            if constexpr (sizeof...(Constructors) > 0) {
                register_constructors<Class, Constructors...>(metadata);
            } else {
                register_constructors<Class, constructor_t<>>(metadata);
            }
        }

        template <auto cpp_method_ptr>
        auto & method(std::string name) {
            using reflector_t = method_reflector_t<decltype(cpp_method_ptr), cpp_method_ptr>;
            static_assert(std::is_same<Class, typename reflector_t::class_t>::value, "Trying to add method from different class");

            metadata.methods.push_back(reflector_t::reflect(std::move(name)));
            return *this;
        }
    };

}
}
}
