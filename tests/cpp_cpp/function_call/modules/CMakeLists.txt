cmake_minimum_required(VERSION 3.4)

include_directories(${INTEROP_ROOT}/framework/cpp_module)

add_library(cpp_cpp.function_call.module1 MODULE module1.cpp)
add_library(cpp_cpp.function_call.module2 MODULE module2.cpp)

SET_TARGET_PROPERTIES(cpp_cpp.function_call.module1 PROPERTIES PREFIX "")
SET_TARGET_PROPERTIES(cpp_cpp.function_call.module2 PROPERTIES PREFIX "")
