cmake_minimum_required(VERSION 3.4)

configure_file(module1.js js_js.function_call.module1.js COPYONLY)
configure_file(module2.js js_js.function_call.module2.js COPYONLY)

include_directories(${INTEROP_ROOT}/framework/cpp_module)
add_library(js_js.function_call.module_cpp MODULE module.cpp)

SET_TARGET_PROPERTIES(js_js.function_call.module_cpp PROPERTIES PREFIX "")
