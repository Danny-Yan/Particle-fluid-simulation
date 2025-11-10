cmake_minimum_required(VERSION 3.16)
project(app)

# include_directories(incl)
set(LIBRARY_SOURCES
    src/engine.cpp
    src/constants.cpp
    src/Helpers/Helper.cpp
    src/Helpers/LTexture.cpp
    src/Helpers/ParticleManager.cpp
    src/Helpers/LTimer.cpp
    src/Colliders/collider.cpp
    src/Colliders/Circle/CircleCollider.cpp
    src/Colliders/Circle/Dot.cpp
    src/Colliders/Circle/Mouse.cpp
)
add_library(${PROJECT_NAME} ${LIBRARY_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE incl)
target_link_libraries(${PROJECT_NAME} PRIVATE external)