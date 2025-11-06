cmake_minimum_required(VERSION 3.16)
project(Particle-fluid-simulation LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lSDL2 -lSDL2_image -lSDL2_ttf -O2")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
include_directories(app)

if (NOT TARGET app)
    add_subdirectory(app)
endif()

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE app SDL2::SDL2)   