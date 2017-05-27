cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE HEADER_FILES include/*.h)
AUX_SOURCE_DIRECTORY(. SOURCE_FILES)

include_directories(${MOSAIC_ROOT}/framework/include_module)

add_library(module2 MODULE ${SOURCE_FILES})

SET_TARGET_PROPERTIES(module2 PROPERTIES PREFIX "")
