cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0075 NEW)
cmake_policy(SET CMP0091 NEW)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lSDL2 -lSDL2_image -lSDL2_ttf -O2")
set( CMAKE_CXX_STANDARD 20 CACHE STRING "" FORCE )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE )
project(Particle-fluid-simulation)

include_directories(app)
add_executable(${PROJECT_NAME} main.cpp)

if (NOT TARGET app)
    add_subdirectory(app)
endif()
add_subdirectory(external)

target_link_libraries(${PROJECT_NAME} app)