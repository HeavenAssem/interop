cmake_minimum_required(VERSION 3.4)

include_directories(${INTEROP_ROOT}/framework/cpp_module
                    ${INTEROP_ROOT}/common/include
                    ${INTEROP_ROOT}/framework/include
                    ${INTEROP_ROOT}/framework/source)

add_executable(tests.node main.cpp)
target_link_libraries(tests.node interop_framework gtest)

set(TESTS_NODE ${CMAKE_CURRENT_BINARY_DIR}/tests.node)

add_subdirectory(cpp_cpp)
add_subdirectory(js_cpp)
add_subdirectory(js_js)
