cmake_minimum_required(VERSION 3.4)

file(GLOB_RECURSE HEADER_FILES include/*.h)
AUX_SOURCE_DIRECTORY(. SOURCE_FILES)

include_directories(${INTEROP_ROOT}/framework/include)

add_executable(cross_module.function_call.root_node ${SOURCE_FILES})

target_link_libraries(cross_module.function_call.root_node interop_framework gtest)

add_test(cross_module.function_call cross_module.function_call.root_node)
