cmake_minimum_required(VERSION 3.4)
project(MosaicFramework)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(MOSAIC_ROOT ${PROJECT_SOURCE_DIR})

option(JS_V8_SUPPORT "Build with JavaScript support (V8 engine dependency)" ON)
option(JS_NODE_SUPPORT "Build with NodeJS support" OFF)

add_subdirectory(common)
add_subdirectory(os_abstraction)
add_subdirectory(framework)
add_subdirectory(tests)
