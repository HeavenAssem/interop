cmake_minimum_required(VERSION 3.4)

include_directories(${INTEROP_ROOT}/framework/cpp_module)

configure_file(module.js cross_language.javascript.objects.module.js COPYONLY)
add_library(cross_language.javascript.objects.module_cpp MODULE module.cpp)

SET_TARGET_PROPERTIES(cross_language.javascript.objects.module_cpp PROPERTIES PREFIX "")
