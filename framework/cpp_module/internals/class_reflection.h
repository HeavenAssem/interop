#pragma once

#include "internals/function_reflection.h"
#include "object_metadata.h"
#include "type_subsystem/mappings.h"

namespace interop {
namespace internals {
namespace class_reflection {

template <class C>
struct destructor_proxy_t {
    static void call(C * object) { return delete object; }
};

template <typename Class, typename... Args>
struct constructor_proxy_t {
    static Class * call(Args &&... args) { return new Class(std::forward<Args>(args)...); }
};

template <class... Args>
struct constructor_t {
    template <class Class>
    using proxy_t     = constructor_proxy_t<Class, Args...>;
    using reflector_t = function_reflection::arguments_reflector_t<Args...>;
};

template <class Class>
struct constructor_reflector_t {
    template <class Constructor>
    static void reflect(std::vector<constructor_metadata_t> & metadata)
    {
        using proxy_t     = typename Constructor::template proxy_t<Class>;
        using reflector_t = typename Constructor::reflector_t;

        metadata.push_back(constructor_metadata_t{reinterpret_cast<void *>(proxy_t::call),
                                                  reflector_t::arguments()});
    }
};

template <class Class, class... Constructors>
void register_constructors(object_metadata_t & metadata)
{
    fold_m(constructor_reflector_t<Class>::template reflect<Constructors>(metadata.constructors));
}

template <class Class>
void register_destructor(object_metadata_t & metadata)
{
    metadata.destructor.pointer = (void *)destructor_proxy_t<Class>::call;
}

template <typename M, M>
class member_reflector_t;

template <typename R, typename C, typename... Args, R (C::*cpp_method_ptr)(Args...)>
class member_reflector_t<R (C::*)(Args...), cpp_method_ptr> {
    using reflected_t = function_reflection::signature_reflector_t<R, Args...>;
    typedef R (C::*cpp_method_t)(Args...);

    static R c_function(void * object, Args... args)
    {
        auto obj = reinterpret_cast<C *>(object);
        return (obj->*cpp_method_ptr)(args...);
    }

  public:
    using class_t = C;

    static constexpr bool is_method = true;

    static function_metadata_t reflect(std::string name)
    {
        return {
            reinterpret_cast<void *>(c_function),
            nullptr,
            std::move(name),
            reflected_t::arguments(),
            reflected_t::return_type(),
        };
    }
};

template <typename T, typename C, T C::*cpp_member_ptr>
class member_reflector_t<T C::*, cpp_member_ptr> {
    typedef const T & (*cpp_member_getter_t)(const C *);
    typedef void (*cpp_member_setter_t)(C *, T &&);

    static const T & getter(const void * object)
    {
        return reinterpret_cast<const C *>(object)->*cpp_member_ptr;
    }

    static void setter(void * object, T && value)
    {
        reinterpret_cast<C *>(object)->*cpp_member_ptr = std::move(value);
    }

  public:
    using class_t = C;

    static constexpr bool is_method = false;

    static field_metadata_t reflect(std::string name)
    {
        field_metadata_t res;
        res.type   = type_subsystem::enumerate_type<T>();
        res.name   = std::move(name);
        res.size   = sizeof(T);
        res.getter = static_cast<void *>(getter);
        res.setter = static_cast<void *>(setter);
        return res;
    }
};

template <class Class, typename... Constructors>
class class_reflector_t {
    object_metadata_t & metadata;

  public:
    class_reflector_t(object_metadata_t & metadata)
      : metadata(metadata)
    {
        register_destructor<Class>(metadata);
        if constexpr (sizeof...(Constructors) > 0) {
            register_constructors<Class, Constructors...>(metadata);
        } else {
            register_constructors<Class, constructor_t<>>(metadata);
        }
    }

    template <auto cpp_member_ptr>
    auto & member(std::string name)
    {
        using reflector_t = member_reflector_t<decltype(cpp_member_ptr), cpp_member_ptr>;
        static_assert(std::is_same<Class, typename reflector_t::class_t>::value,
                      "Trying to add member from a different class");

        if constexpr (reflector_t::is_method) {
            metadata.methods.push_back(reflector_t::reflect(std::move(name)));
        } else {
            metadata.fields.push_back(reflector_t::reflect(std::move(name)));
        }
        return *this;
    }
};

} // namespace class_reflection
} // namespace internals
} // namespace interop
