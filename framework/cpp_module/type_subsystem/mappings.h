#pragma once

#include "../cpp_module/declarations.h"
#include "declarations.h"

namespace interop {
namespace type_subsystem {

template <typename Ty>
struct map_type_enum {
    static constexpr type_e type = type_e::Unsupported;
};
template <typename Ty>
struct map_type_enum<Ty *> {
    static constexpr type_e type = type_e::Pointer;
};
template <>
struct map_type_enum<void> {
    static constexpr type_e type = type_e::Void;
};

#define XX(NAME, TYPE, _1)                                                                         \
    template <>                                                                                    \
    struct map_type_enum<TYPE> {                                                                   \
        static constexpr type_e type = type_e::NAME;                                               \
    };
INTEROP_TYPE_ENUM(XX)
#undef XX

template <typename T>
constexpr type_e enumerate_type()
{
    constexpr auto type = map_type_enum<typename std::decay<T>::type>::type;
    static_assert(type != type_e::Unsupported, "Unsupported type");
    return type;
}

val_t initialized_val(type_e);

inline std::string to_string(type_e type)
{
    switch (type) {
    case type_e::Unsupported:
        return "<unsupported>";
    case type_e::Pointer:
        return "pointer";
    case type_e::Void:
        return "void";

#define XX(NAME, _1, ALIAS)                                                                        \
    case type_e::NAME:                                                                             \
        return ALIAS;
        INTEROP_TYPE_ENUM(XX)
#undef XX
    }
}
} // namespace type_subsystem
} // namespace interop
