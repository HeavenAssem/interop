//
// Created by islam on 14.01.18.
//

#pragma once

#define interop_link _interop_module_link
#define interop_register _interop_module_register

#define interop_module_metadata_m _interop_module_metadata
#define interop_framework_abi_version_m _interop_framework_abi_version

#define __interop_stringify_impl_m(x) #x
#define __interop_stringify_m(x) __interop_stringify_impl_m(x)

const auto interop_module_link_c           = __interop_stringify_m(interop_link);
const auto interop_module_register_c       = __interop_stringify_m(interop_register);
const auto interop_module_metadata_c       = __interop_stringify_m(interop_module_metadata_m);
const auto interop_framework_abi_version_c = __interop_stringify_m(interop_framework_abi_version_m);
