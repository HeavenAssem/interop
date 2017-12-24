cmake_minimum_required(VERSION 3.4)

file(GLOB_RECURSE HEADER_FILES include/*.h)
AUX_SOURCE_DIRECTORY(. SOURCE_FILES)

include_directories(${MOSAIC_ROOT}/framework/include)

add_executable(cross_language.javascript.function_call.root_node ${SOURCE_FILES})

target_link_libraries(cross_language.javascript.function_call.root_node mosaic_framework gtest)
