cmake_minimum_required(VERSION 2.8)
project(MosaicFramework)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -Wno-c++1z-extensions)

set(MOSAIC_ROOT ${PROJECT_SOURCE_DIR})

add_subdirectory(common)
add_subdirectory(platform_abstraction)
add_subdirectory(framework)
add_subdirectory(tests)
