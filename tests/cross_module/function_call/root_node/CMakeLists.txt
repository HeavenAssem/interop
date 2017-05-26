cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE HEADER_FILES include/*.h)
AUX_SOURCE_DIRECTORY(. SOURCE_FILES)

include_directories(${MOSAIC_ROOT}/framework/include)

add_executable(root_node ${SOURCE_FILES})

target_link_libraries(root_node mosaic_framework)
