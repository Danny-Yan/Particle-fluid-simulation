cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0075 NEW)
cmake_policy(SET CMP0091 NEW)

project(particle-simulation)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set( CMAKE_CXX_STANDARD 20 CACHE STRING "" FORCE )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE )

if(MSVC)
  enable_language(ASM_MASM)
endif()

# include_directories(app)
add_executable(main main.cpp)
set_target_properties(main PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})

add_subdirectory(app)
add_subdirectory(external)
target_link_libraries(main PRIVATE app external)