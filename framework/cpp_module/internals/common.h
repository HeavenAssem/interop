#pragma once

#include "type_metadata.h"
#include "type_subsystem/type.h"

#include <vector>

#if __cplusplus < 201103L
static_assert(false, "Pre C++11 standards are not supported");
#endif
#if __cplusplus >= 201402L
#define CPP_14
#endif
#if __cplusplus >= 201703L
#define CPP_17
#endif

namespace interop {
namespace internals {
namespace meta {

template <typename... Ts>
struct forbidden_instantiation {
    static_assert(sizeof...(Ts) < 0, "forbidden instantiation");
};

template <class T>
type_metadata_t describe_type()
{
    return {enumerate_type<T>(), sizeof(T)};
}

template <>
type_metadata_t describe_type<void>()
{
    return {enumerate_type<void>(), 0};
}

template <class T>
void describe_type(std::vector<type_metadata_t> & metadata)
{
    metadata.emplace_back(describe_type<T>());
}

// clang-format off
#ifdef CPP_17
#define fold_m(expr) ((void)expr, ...)
#else
#define fold_m(expr) { int dummy[] = {0, ((void)expr, 0)...}; }
#endif
// clang-format on

} // namespace meta
} // namespace internals
} // namespace interop
