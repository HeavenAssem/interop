#pragma once

#include "../cpp_module/declarations.h"

namespace interop {
class node_t;
class module_view_t;
class internal_module_t;
class native_module_t;

class platform_t;
class platform_factory_t;
class platform_function_t;

struct platform_configuration_t;

using module_ptr_t            = std::unique_ptr<internal_module_t>;
using platform_ptr_t          = std::unique_ptr<platform_t>;
using platform_factory_ptr    = std::shared_ptr<platform_factory_t>;
using platform_function_ptr_t = std::shared_ptr<platform_function_t>;

using module_id_t = uint16_t;

const module_id_t unassigned_id_c = -1;

struct class_id_t {
    module_id_t module_id = unassigned_id_c;
    uint16_t local_id;
};

inline bool operator==(const class_id_t & lhs, const class_id_t & rhs)
{
    return lhs.module_id == rhs.module_id && lhs.local_id == rhs.local_id;
}

struct class_id_hash_t {
    std::size_t operator()(const class_id_t & key) const
    {
        union class_id_as_t {
            class_id_t decomposed;
            uint32_t composed;

            static_assert(sizeof(decomposed) == sizeof(composed),
                          "different sizes of class id representations");
        };

        return std::hash<uint32_t>{}(class_id_as_t{key}.composed);
    }
};

} // namespace interop
