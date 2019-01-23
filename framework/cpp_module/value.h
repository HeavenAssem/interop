#pragma once

#include "type_subsystem/type.h"

#include <any.h>
#include <invariant.h>

#include <string>

namespace interop {
struct value_t;
namespace details {
struct i_value_type_wrapper_t: i_type_wrapper_t {
    virtual type_e get_enumerated_type() const            = 0;
    virtual bool is_convertible(type_e) const             = 0;
    virtual void convert_to_type(type_e, value_t &) const = 0;
};

template <class T, class Interface>
struct value_type_wrapper_t: type_wrapper_t<T, Interface> {
    type_e get_enumerated_type() const override { return enumerate_type<T>(); }
    bool is_convertible(type_e type) const override
    {
        switch (type) {
        case type_e::Void:
            return std::is_convertible<T, void>::value;
        case type_e::Pointer:
            return std::is_convertible<T, void *>::value;
#define XX(NAME, TYPE, _1)                                                                         \
    case type_e::NAME:                                                                             \
        return std::is_convertible<T, TYPE>::value;
            INTEROP_TYPE_ENUM(XX)
#undef XX
        default:
            throw typing_error_t("unsupported type for value_t conversion");
        }
    }
    void convert_to_type(type_e, value_t &) const override;
};

struct forbidding_allocator_t {
    inline static void * allocate(size_t)
    {
        INTEROP_INVARIANT(false, "@allocate: dynamic allocation of value is forbidden");
        return nullptr;
    }

    inline static void free(void *)
    {
        INTEROP_INVARIANT(false, "@free: dynamic allocation of value is forbidden");
    }
};

using value_type_eraser_t =
    type_eraser_t<details::i_value_type_wrapper_t, details::value_type_wrapper_t>;
using value_base_t = any_basic_t<sizeof(std::string), value_type_eraser_t, forbidding_allocator_t>;
} // namespace details

struct value_t final: public details::value_base_t {
    using details::value_base_t::any_basic_t;

    explicit value_t(type_e);

    type_e enumerated_type() const
    {
        static const auto default_type = type_e::Void;

        return wrapped_type ? wrapped_type->get_enumerated_type() : default_type;
    }

    void convert_to(type_e type)
    {
        if (enumerated_type() == type) {
            return;
        }
        INTEROP_INVARIANT(wrapped_type, "no wrapped type");
        wrapped_type->convert_to_type(type, *this);
    }
};

namespace details {
template <typename To, typename From>
inline To convert(From && from)
{
    return std::forward<From>(from);
}

template <class T, class Interface>
void value_type_wrapper_t<T, Interface>::convert_to_type(type_e type, value_t & value) const
{
    if (!is_convertible(type)) {
        throw typing_error_t(this->get_name() + " is not convertible to " + to_string(type));
    }

    switch (type) {
    case type_e::Void:
        return value.clear();
    case type_e::Pointer:
        if constexpr (std::is_convertible<T, void *>::value) {
            value = convert<void *>(value.as<T>());
        } else {
            INTEROP_INVARIANT(false, "not convertible but should be");
        }
        return;
#define XX(NAME, TYPE, _1)                                                                         \
    case type_e::NAME:                                                                             \
        if constexpr (std::is_convertible<T, TYPE>::value) {                                       \
            value = convert<TYPE>(value.as<T>());                                                  \
        } else {                                                                                   \
            INTEROP_INVARIANT(false, "not convertible but should be");                             \
        }                                                                                          \
        return;
        INTEROP_TYPE_ENUM(XX)
#undef XX
    default:
        throw typing_error_t("unsupported type for value_t conversion");
    }
}

} // namespace details
} // namespace interop
