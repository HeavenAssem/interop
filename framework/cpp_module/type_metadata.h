//
// Created by islam on 27.05.17.
//

#pragma once

#include <string>


namespace mosaic {
    enum class type_enum: uint8_t {
        TE_UNSUPPORTED,
        TE_VOID,
        TE_BOOL,
        TE_CHAR,
        TE_SHORT,
        TE_FLOAT,
        TE_DOUBLE,
        TE_INT,
        TE_LONG,
        TE_LONG_LONG,
        TE_POINTER,
        TE_STRING
    };

    namespace details {
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
        MAP_ENUM(std::string, TE_STRING)    // <- TODO: change: will break abi compatibility

#undef MAP_ENUM
    }

    template <typename T>
    constexpr type_enum enumerate_type() {
        constexpr auto type = details::map_enum<typename std::decay<T>::type>::type;
        static_assert(type != type_enum::TE_UNSUPPORTED, "Unsupported type");
        return type;
    }

    inline std::string to_string(type_enum type) {
        switch (type) {
            case type_enum::TE_UNSUPPORTED:     return "<unsupported>";
            case type_enum::TE_VOID:            return "void";
            case type_enum::TE_BOOL:            return "bool";
            case type_enum::TE_CHAR:            return "char";
            case type_enum::TE_SHORT:           return "short";
            case type_enum::TE_FLOAT:           return "float";
            case type_enum::TE_DOUBLE:          return "double";
            case type_enum::TE_INT:             return "int";
            case type_enum::TE_LONG:            return "long";
            case type_enum::TE_LONG_LONG:       return "long long";
            case type_enum::TE_POINTER:         return "pointer";
            case type_enum::TE_STRING:          return "string";
        }
    }

    struct type_metadata {
        type_enum type;
        std::size_t size;
    };

    struct variable_metadata: type_metadata {
        std::string name;
        uint16_t shift;
    };
}
