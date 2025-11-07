cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0075 NEW)
cmake_policy(SET CMP0091 NEW)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lSDL2 -lSDL2_image -lSDL2_ttf -O2")
set( CMAKE_CXX_STANDARD 20 CACHE STRING "" FORCE )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE )
project(Particle-fluid-simulation LANGUAGES CXX)
include_directories(app)

add_executable(${PROJECT_NAME} main.cpp)

set_target_properties( ${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME} )
add_custom_command( TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<CONFIG>/assets
    )

if (NOT TARGET app)
    add_subdirectory(app)
endif()
add_subdirectory(external)

if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set( CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" )
    set_property( DIRECTORY PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME} )
    set_property( TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE true )
endif()