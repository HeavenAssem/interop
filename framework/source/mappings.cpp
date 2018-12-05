#include "type_subsystem/mappings.h"

#include <exceptions.h>

namespace interop {
namespace type_subsystem {
val_t initialized_val(type_e type)
{
    val_t val;
    switch (type) {
    case type_e::Void:
        // pass
        break;
    case type_e::Pointer:
        val.emplace<void *, true>();
        break;
#define XX(NAME, TYPE, _1)                                                                         \
    case type_e::NAME:                                                                             \
        val.emplace<TYPE, true>();                                                                 \
        break;
        INTEROP_TYPE_ENUM(XX)
#undef XX
    default:
        throw typing_error_t("unsupported type for val_t initializaton");
    }
    return val;
}
} // namespace type_subsystem
} // namespace interop
