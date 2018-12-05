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
namespace type_subsystem {
enum class type_e : uint8_t {
#define XX(NAME, _1, _2) NAME,
    Unsupported,
    Pointer,
    Void,
    INTEROP_TYPE_ENUM(XX)
#undef XX
};
} // namespace type_subsystem

using type_subsystem::type_e;

} // namespace interop
