cmake_minimum_required(VERSION 2.8)

file(GLOB_RECURSE HEADER_FILES include/*.h include_module/*.h)
AUX_SOURCE_DIRECTORY(source SOURCE_FILES)

include_directories(include include_module ../platform_abstraction/include ../common/include)

add_library(mosaic_framework SHARED ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(mosaic_framework platform_abstraction boost_system boost_filesystem)