#pragma once

#include <type_traits>

namespace interop {

template <typename Base, typename Derived>
inline constexpr bool is_derived_from =
    std::is_base_of<Base, Derived>::value && !std::is_same<Base, Derived>::value;

template <typename T>
inline constexpr bool is_function_pointer =
    std::is_pointer<T>::value ? std::is_function<typename std::remove_pointer<T>::type>::value
                              : false;

} // namespace interop
