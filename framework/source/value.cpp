#include "value.h"

namespace interop {
value_t::value_t(type_e type)
{
    switch (type) {
    case type_e::Void:
        // pass
        break;
    case type_e::Pointer:
        emplace_in_place<void *>();
        break;
#define XX(NAME, TYPE, _1)                                                                         \
    case type_e::NAME:                                                                             \
        emplace_in_place<TYPE>();                                                                  \
        break;
        INTEROP_TYPE_ENUM(XX)
#undef XX
    default:
        throw typing_error_t("unsupported type for value_t initializaton");
    }
}
} // namespace interop
