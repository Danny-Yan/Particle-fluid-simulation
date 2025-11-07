set( BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE )

# SDL
# https://github.com/libsdl-org/SDL
set( SDL_LIBC ON CACHE BOOL "" FORCE )
set( SDL_TEST OFF CACHE BOOL "" FORCE )
add_subdirectory( sdl )
target_link_libraries( ${PROJECT_NAME} SDL2::SDL2main SDL2::SDL2-static )

# SDL_image
# https://github.com/libsdl-org/SDL_image
set( SDL2IMAGE_SAMPLES OFF CACHE BOOL "" FORCE )
set( SDL2IMAGE_INSTALL OFF CACHE BOOL "" FORCE )
set( SDL2IMAGE_VENDORED ON CACHE BOOL "" FORCE )
set( SDL2IMAGE_DEPS_SHARED OFF CACHE BOOL "" FORCE )
add_subdirectory( sdl_image )
target_link_libraries( ${PROJECT_NAME} SDL2_image::SDL2_image-static )