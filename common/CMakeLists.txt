cmake_minimum_required(VERSION 2.8)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

file(GLOB_RECURSE HEADER_FILES include/*.h)
AUX_SOURCE_DIRECTORY(source SOURCE_FILES)

include_directories(include ../external/rlutil)

add_library(common STATIC ${SOURCE_FILES} ${HEADER_FILES})
