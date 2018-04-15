cmake_minimum_required(VERSION 3.4)

include_directories(../framework/cpp_module ../common/include)

enable_testing()

add_subdirectory(cross_module)
add_subdirectory(cross_language)
