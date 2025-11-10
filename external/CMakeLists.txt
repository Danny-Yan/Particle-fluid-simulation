set( BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )
project(external)
add_library( ${PROJECT_NAME} INTERFACE )
# SDL
# https://github.com/libsdl-org/SDL
set( SDL_LIBC ON CACHE BOOL "" FORCE )
set( SDL_TEST OFF CACHE BOOL "" FORCE )
add_subdirectory( SDL )
target_link_libraries( ${PROJECT_NAME} INTERFACE SDL3::SDL3 )

# SDL_image
# https://github.com/libsdl-org/SDL_image
set( SDL3IMAGE_SAMPLES OFF CACHE BOOL "" FORCE )
set( SDL3IMAGE_INSTALL OFF CACHE BOOL "" FORCE )
set( SDL3IMAGE_VENDORED ON CACHE BOOL "" FORCE )
set( SDL3IMAGE_DEPS_SHARED OFF CACHE BOOL "" FORCE )
add_subdirectory( SDL_image )
target_link_libraries( ${PROJECT_NAME} INTERFACE SDL3_image::SDL3_image-static )