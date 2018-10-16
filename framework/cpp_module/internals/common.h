#include "type_metadata.h"

#include <vector>

namespace interop {
namespace internals {
namespace meta {

template <class T>
type_metadata_t describe_type()
{
    return type_metadata_t{enumerate_type<T>(), sizeof(T)};
}

template <>
type_metadata_t describe_type<void>()
{
    return type_metadata_t{enumerate_type<void>(), 0};
}

// TODO: maybe delete this shit
template <class operation, class T, class... Ts, class... Args>
static void iterate_impl(Args &&... args)
{
    operation::template exec<T>(std::forward<Args>(args)...);
    if constexpr (sizeof...(Ts) > 0) {
        iterate_impl<operation, Ts...>(std::forward<Args>(args)...);
    }
}

template <class operation, class... Ts>
struct iterate {
    template <class... Args>
    static void exec(Args &&... args)
    {
        if constexpr (sizeof...(Ts) > 0) {
            iterate_impl<operation, Ts...>(std::forward<Args>(args)...);
        }
    }
};

struct describe_type_op {
    template <class T>
    static void exec(std::vector<type_metadata_t> & metadata)
    {
        metadata.push_back(describe_type<T>());
    }
};

template <class... Ts>
static void describe_types(std::vector<type_metadata_t> & metadata)
{
    iterate<describe_type_op, Ts...>::exec(metadata);
}

} // namespace meta
} // namespace internals
} // namespace interop
