cmake_minimum_required(VERSION 3.4)

include_directories(${MOSAIC_ROOT}/framework/cpp_module)

configure_file(module.js module.js COPYONLY)
add_library(cross_language.javascript.function_call.module2 MODULE module2.cpp)

SET_TARGET_PROPERTIES(cross_language.javascript.function_call.module2 PROPERTIES PREFIX "")
