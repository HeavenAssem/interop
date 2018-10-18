cmake_minimum_required(VERSION 3.4)

include_directories(../framework/cpp_module ../common/include)

include_directories(${INTEROP_ROOT}/framework/include)
add_executable(tests.node main.cpp)
target_link_libraries(tests.node interop_framework gtest)

set(TESTS_NODE ${CMAKE_CURRENT_BINARY_DIR}/tests.node)

add_subdirectory(cross_module)
add_subdirectory(cross_language)
