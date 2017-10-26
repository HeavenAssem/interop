cmake_minimum_required(VERSION 3.4)

file(GLOB HEADER_FILES include/*.h)

if(WIN32)
    AUX_SOURCE_DIRECTORY(source/windows SOURCE_FILES)
    file(GLOB PLATFORM_HEADER_FILES utility/windows/*.h)
    include_directories(utility/windows)
elseif(UNIX)
    AUX_SOURCE_DIRECTORY(source/linux SOURCE_FILES)
    file(GLOB PLATFORM_HEADER_FILES utility/linux/*.h)
    include_directories(utility/linux)
    set(PLATFORM_LIBRARIES dl)
endif()

AUX_SOURCE_DIRECTORY(source SOURCE_FILES)

include_directories(include ../common/include)

add_library(platform_abstraction SHARED ${SOURCE_FILES} ${HEADER_FILES} ${PLATFORM_HEADER_FILES})

target_link_libraries(platform_abstraction common ${PLATFORM_LIBRARIES})
