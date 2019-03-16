#pragma once

#include <type_traits>

namespace interop {
template <typename Base, typename Derived>
struct is_derived_from
  : public std::integral_constant<bool, std::is_base_of<Base, Derived>::value &&
                                            !std::is_same<Base, Derived>::value> {};

} // namespace interop
