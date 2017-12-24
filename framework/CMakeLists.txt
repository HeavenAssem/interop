cmake_minimum_required(VERSION 3.4)

file(GLOB_RECURSE HEADER_FILES include/*.h include_module/*.h)
AUX_SOURCE_DIRECTORY(source SOURCE_FILES)

set(LIBRARIES mosaic_framework platform_abstraction dyncall_s boost_system boost_filesystem)

option(JS_V8_SUPPORT "Build with JavaScript support (V8 engine dependency)" ON)

if(JS_V8_SUPPORT)
    list(APPEND SOURCE_FILES source/platforms/javascript_v8.cpp)
    list(APPEND LIBRARIES c++ v8 v8_libplatform)
endif(JS_V8_SUPPORT)

include_directories(include include_module ../platform_abstraction/include ../common/include)

add_library(mosaic_framework SHARED ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(${LIBRARIES})