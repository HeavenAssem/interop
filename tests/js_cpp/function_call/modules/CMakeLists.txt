cmake_minimum_required(VERSION 3.4)

include_directories(${INTEROP_ROOT}/framework/cpp_module)

configure_file(module.js js_cpp.function_call.module.js COPYONLY)
add_library(js_cpp.function_call.module_cpp MODULE module.cpp)

SET_TARGET_PROPERTIES(js_cpp.function_call.module_cpp PROPERTIES PREFIX "")
