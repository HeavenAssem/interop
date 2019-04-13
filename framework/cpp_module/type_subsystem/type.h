#pragma once

#include <string>

// ugly, but robust
#define INTEROP_TYPE_ENUM(XX)            /*|no clang-format|*/                                     \
    XX(Bool, bool, "bool")               /*|no clang-format|*/                                     \
    XX(Char, char, "char")               /*|no clang-format|*/                                     \
    XX(Short, short, "short")            /*|no clang-format|*/                                     \
    XX(Float, float, "float")            /*|no clang-format|*/                                     \
    XX(Double, double, "double")         /*|no clang-format|*/                                     \
    XX(Int, int, "int")                  /*|no clang-format|*/                                     \
    XX(Long, long, "long")               /*|no clang-format|*/                                     \
    XX(LongLong, long long, "long long") /*|no clang-format|*/                                     \
    XX(String, std::string, "string")    /*TODO: change: will break abi compatibility*/

namespace interop {

enum class type_e : uint8_t {
#define XX(NAME, _1, _2) NAME,
    Unsupported,
    Pointer,
    Void,
    INTEROP_TYPE_ENUM(XX)
#undef XX
};

namespace details {
template <template <typename> typename Predicate>
inline auto forward_predicate(type_e type)
{
    switch (type) {
    case type_e::Void:
        return Predicate<void>::value;
    case type_e::Pointer:
        return Predicate<void *>::value;
#define XX(NAME, TYPE, _1)                                                                         \
    case type_e::NAME:                                                                             \
        return Predicate<TYPE>::value;
        INTEROP_TYPE_ENUM(XX)
#undef XX
    default:
        return typename Predicate<void>::value_type{};
    }
}

template <typename T>
struct get_size_t {
    using value_type                  = size_t;
    constexpr static value_type value = sizeof(T);
};

template <>
struct get_size_t<void> {
    using value_type                  = size_t;
    constexpr static value_type value = 0;
};
} // namespace details

inline bool is_integral(type_e type) { return details::forward_predicate<std::is_integral>(type); }

inline bool is_floating_point(type_e type)
{
    return details::forward_predicate<std::is_floating_point>(type);
}

inline size_t size(type_e type) { return details::forward_predicate<details::get_size_t>(type); }

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

inline std::string to_string(type_e type)
{
    // TODO: eliminate allocations
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

    return "<unsupported>";
}

} // namespace interop
