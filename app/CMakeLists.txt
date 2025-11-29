project(app)

# Include source files
set(LIBRARY_SOURCES
    incl/engine.h
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

# Link incl and external directories
add_library(${PROJECT_NAME} ${LIBRARY_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE incl)
target_link_libraries(${PROJECT_NAME} PRIVATE external)