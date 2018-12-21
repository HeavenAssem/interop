// #include "type_subsystem/implicit_conversion.h"
// #include "type_subsystem/type.h"

// #include <cassert>

// using namespace std;

// namespace interop {
// namespace type_subsystem {
// namespace {
// bool check_is_same(const val_t & value, type_e type)
// {
//     switch (type) {
//     case type_e::Void:
//         // pass
//         return value.empty();
//     case type_e::Pointer:
//         return value.is_pointer();
// #define XX(NAME, TYPE, _1) \
//     case type_e::NAME: \
//         return val.is<TYPE>(); \ INTEROP_TYPE_ENUM(XX)
// #undef XX
//     default:
//         throw typing_error_t("unsupported type for val_t");
//     }
// }

// void convert(val_t & value, type_e type)
// {
//     switch (type) {
//     case type_e::Void:
//         // pass
//         value.clear();
//         return;
//     case type_e::Pointer:
//         assert(false);
//         return;
// #define XX(NAME, TYPE, _1) \
//     case type_e::NAME: \
//         val.is<TYPE>(); \ return; INTEROP_TYPE_ENUM(XX)
// #undef XX
//     default:
//         throw typing_error_t("unsupported type for val_t");
//     }
// }

// void perform_implicit_conversion_impl(val_t & value, type_e to_type)
// {
//     auto from_type = value.get_enumerated_type();

//     if (from_type == to_type) {
//         return;
//     }

//     if (value.empty()) {
//         throw typing_error_t("value is empty");
//     }

//     if (to_type == type_e::Pointer) {
//         throw typing_error_t("conversion from non-pointer to pointer");
//     }

//     if (size(from_type) > size(to_type)) {
//         throw typing_error_t(
//             "converion to smaller type"); // TODO: since we're doing it in runtime, consider
//                                           // allowing conversion if value will fit
//     }

//     if (is_integral(from_type) && is_integral(to_type)) {
//     }

//     if (is_floating_point(from_type) && floating_point(to_type)) {
//     }
// }

// } // namespace

// void perform_implicit_conversion(val_t & value, type_e type)
// {
//     try {
//         perform_implicit_conversion_impl(value, type);
//     } catch (const typing_error_t & e) {
//         throw typing_error_t("unable to perform implicit type conversion: "s + e.what());
//     }
// }
// } // namespace type_subsystem
// } // namespace interop
