cmake_minimum_required(VERSION 3.4)
project(MosaicFramework)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(MOSAIC_ROOT ${PROJECT_SOURCE_DIR})

add_subdirectory(common)
add_subdirectory(os_abstraction)
add_subdirectory(framework)
add_subdirectory(tests)
