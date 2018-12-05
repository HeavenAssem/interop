#pragma once

#include <string>

namespace interop {
namespace utils {
std::string demangle(const char * mangledName);

template <class T>
std::string name_of_type()
{
    return demangle(typeid(T).name());
}

template <class T>
std::string name_of_type(T && obj)
{
    return demangle(typeid(std::forward<T>(obj)).name());
}
} // namespace utils
} // namespace interop
