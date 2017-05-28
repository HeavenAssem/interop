cmake_minimum_required(VERSION 2.8)

include_directories(${MOSAIC_ROOT}/framework/include_module)

add_library(module1 MODULE module1.cpp)
add_library(module2 MODULE module2.cpp)

SET_TARGET_PROPERTIES(module1 PROPERTIES PREFIX "")
SET_TARGET_PROPERTIES(module2 PROPERTIES PREFIX "")