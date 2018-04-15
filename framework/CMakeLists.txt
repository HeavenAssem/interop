cmake_minimum_required(VERSION 3.4)

file(GLOB_RECURSE HEADER_FILES include/*.h cpp_module/*.h)
AUX_SOURCE_DIRECTORY(source SOURCE_FILES)

set(LIBRARIES os_abstraction jsoncpp dyncall_s boost_system boost_filesystem)

option(JS_V8_SUPPORT "Build with JavaScript support (V8 engine dependency)" ON)
option(JS_NODE_SUPPORT "Build with NodeJS support" OFF)

#if(JS_V8_SUPPORT)
    AUX_SOURCE_DIRECTORY(source/platforms/javascript_v8 SOURCE_FILES)
    list(APPEND LIBRARIES /usr/lib/v8/libc++.so v8 v8_libplatform)
#endif(JS_V8_SUPPORT)

if(JS_NODE_SUPPORT)
    list(APPEND SOURCE_FILES source/platforms/javascript_node.cpp)
    list(APPEND LIBRARIES node)
endif(JS_NODE_SUPPORT)

include_directories(
        include
        cpp_module
        ${CMAKE_SOURCE_DIR}/os_abstraction/include
        ${CMAKE_SOURCE_DIR}/common/include
)

add_library(mosaic_framework SHARED ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(mosaic_framework ${LIBRARIES})
