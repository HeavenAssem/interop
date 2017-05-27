//
// Created by islam on 27.05.17.
//

#pragma once

#include <string>
#include <tcl.h>


namespace mosaic {
    enum class type_enum: uint8_t {TE_UNSUPPORTED, TE_VOID, TE_BOOL, TE_CHAR, TE_SHORT, TE_FLOAT, TE_DOUBLE, TE_INT, TE_LONG, TE_LONG_LONG, TE_POINTER};

    namespace detail {
        template <typename Ty> struct map_enum { static constexpr type_enum type = type_enum::TE_UNSUPPORTED; };
        template <typename Ty> struct map_enum <Ty*> { static constexpr type_enum type = type_enum::TE_POINTER; };

#define MAP_ENUM(T, E) template <> struct map_enum <T> { static constexpr type_enum type = type_enum::E; };

        MAP_ENUM(void, TE_VOID)
        MAP_ENUM(bool, TE_BOOL)
        MAP_ENUM(char, TE_CHAR)
        MAP_ENUM(short, TE_SHORT)
        MAP_ENUM(float, TE_FLOAT)
        MAP_ENUM(double, TE_DOUBLE)
        MAP_ENUM(int, TE_INT)
        MAP_ENUM(long, TE_LONG)
        MAP_ENUM(long long, TE_LONG_LONG)

#undef MAP_ENUM
    }

    template <typename T>
    constexpr type_enum enumerate_type() {
        return detail::map_enum<typename std::decay<T>::type>::type;
    }

    struct type_metadata {
        type_enum type;
        std::size_t size;
    };
}
