#pragma once

#include "function_reflection.h"
#include "object_metadata.h"
#include "type_subsystem/type.h"

namespace interop {
namespace internals {
namespace class_reflection {
namespace details {

template <typename MemberPtr, MemberPtr member_ptr, typename C, typename R, typename... Args,
          size_t... Indices>
universal_wrapper_t wrap_universally(std::index_sequence<Indices...>)
{
    return [](void * object_ptr, arg_pack_t args) -> val_t {
        auto & object = *static_cast<C *>(object_ptr);
        if constexpr (std::is_void<R>::value) {
            (object.*member_ptr)(args[Indices].as<Args>()...);
            return {};
        } else {
            return (object.*member_ptr)(args[Indices].as<Args>()...);
        }
    };
}
template <typename MemberPtr, MemberPtr member_ptr, typename C, typename R, typename... Args>
universal_wrapper_t wrap_universally()
{
    return wrap_universally<MemberPtr, member_ptr, C, R, Args...>(
        std::index_sequence_for<Args...>());
}
} // namespace details

template <typename Class>
struct destructor_proxy_t {
    static void call(void * object) { return delete static_cast<Class *>(object); }
};

template <typename Class, typename... Args>
struct constructor_proxy_t {
    static Class * call(Args... args) { return new Class(std::forward<Args>(args)...); }
};

template <class... Args>
struct constructor_t {
    template <class Class>
    struct instantiated_t {
        using proxy_t     = constructor_proxy_t<Class, Args...>;
        using reflector_t = function_reflection::function_reflector_t<decltype(&proxy_t::call)>;
    };
};

template <class Class>
struct constructor_reflector_t {
    template <class Constructor>
    static void reflect(std::vector<constructor_metadata_t> & metadata)
    {
        using instantiated_t = typename Constructor::template instantiated_t<Class>;
        using proxy_t        = typename instantiated_t::proxy_t;
        using reflector_t    = typename instantiated_t::reflector_t;

        metadata.push_back(constructor_metadata_t{reinterpret_cast<void *>(proxy_t::call),
                                                  reflector_t::wrap_universally(),
                                                  reflector_t::arguments()});
    }
};

template <class Class, class... Constructors>
void register_constructors(object_metadata_t & metadata)
{
    unpack_m(constructor_reflector_t<Class>::template reflect<Constructors>(metadata.constructors));
}

template <class Class>
void register_destructor(object_metadata_t & metadata)
{
    metadata.destructor.pointer = destructor_proxy_t<Class>::call;
}

template <typename M, M>
class member_reflector_t;

/**
 * @brief metadata generator for non-const methods
 */
template <typename R, typename C, typename... Args, R (C::*cpp_method_ptr)(Args...)>
class member_reflector_t<R (C::*)(Args...), cpp_method_ptr> {
    using reflected_t  = function_reflection::signature_reflector_t<R, Args...>;
    using cpp_method_t = decltype(cpp_method_ptr);

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
            details::wrap_universally<cpp_method_t, cpp_method_ptr, C, R, Args...>(),
            std::move(name),
            reflected_t::arguments(),
            reflected_t::return_type(),
        };
    }
};

// TODO: remove duplication
/**
 * @brief metadata generator for const methods
 */
template <typename R, typename C, typename... Args, R (C::*cpp_method_ptr)(Args...) const>
class member_reflector_t<R (C::*)(Args...) const, cpp_method_ptr> {
    using reflected_t  = function_reflection::signature_reflector_t<R, Args...>;
    using cpp_method_t = decltype(cpp_method_ptr);

    static R c_function(void * object, Args... args)
    {
        auto obj = reinterpret_cast<const C *>(object);
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
            details::wrap_universally<cpp_method_t, cpp_method_ptr, C, R, Args...>(),
            std::move(name),
            reflected_t::arguments(),
            reflected_t::return_type(),
        };
    }
};

/**
 * @brief metadata generator for const fields
 */
template <typename T, typename C, const T C::*cpp_member_ptr>
class member_reflector_t<const T C::*, cpp_member_ptr> {
    typedef const T & (*cpp_member_getter_t)(const C *);

    static const T & getter(const void * object)
    {
        return reinterpret_cast<const C *>(object)->*cpp_member_ptr;
    }

    static val_t dynamic_getter(const void * object) { return getter(object); }

  public:
    using class_t = C;

    static constexpr bool is_method = false;

    static field_metadata_t reflect(std::string name)
    {
        field_metadata_t res;
        res.type           = enumerate_type<T>();
        res.name           = std::move(name);
        res.size           = sizeof(T);
        res.getter         = reinterpret_cast<void *>(getter);
        res.dynamic_getter = dynamic_getter;
        return res;
    }
};

/**
 * @brief metadata generator for non-const fields
 */
template <typename T, typename C, T C::*cpp_member_ptr>
class member_reflector_t<T C::*, cpp_member_ptr> {
    typedef const T & (*cpp_member_getter_t)(const C *);
    typedef void (*cpp_member_setter_t)(C *, T &&);

    static const T & getter(const void * object)
    {
        return reinterpret_cast<const C *>(object)->*cpp_member_ptr;
    }

    static val_t dynamic_getter(const void * object) { return getter(object); }

    static void setter(void * object, T && value)
    {
        reinterpret_cast<C *>(object)->*cpp_member_ptr = std::move(value);
    }

    static void dynamic_setter(void * object, val_t value)
    {
        return setter(object, std::move(value.as<T>()));
    }

  public:
    using class_t = C;

    static constexpr bool is_method = false;

    static field_metadata_t reflect(std::string name)
    {
        field_metadata_t res;
        res.type           = enumerate_type<T>();
        res.name           = std::move(name);
        res.size           = sizeof(T);
        res.getter         = reinterpret_cast<void *>(getter);
        res.dynamic_getter = dynamic_getter;
        res.setter         = reinterpret_cast<void *>(setter);
        res.dynamic_setter = dynamic_setter;
        return res;
    }
};

/**
 * @brief metadata generator and wrapper for class
 */
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
