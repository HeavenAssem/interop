#pragma once

#include <type_traits>

#define not_lvalue_ref_m(T)                                                                        \
    typename = typename std::enable_if<!std::is_lvalue_reference<T>::value>::type
#define of_other_type_m(T) typename = typename std::enable_if<!is_any<T>::value>::type
#define not_void_m(T) typename = typename std::enable_if<!std::is_void<T>::value>::type
#define is_void_m(T) typename = typename std::enable_if<std::is_void<T>::value>::type
