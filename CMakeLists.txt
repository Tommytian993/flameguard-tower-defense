cmake_minimum_required(VERSION 3.28)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled main.cpp
        manager.h
        game_manager.h)

# Use sdl2-config to get the paths for SDL2
execute_process(COMMAND sdl2-config --cflags OUTPUT_VARIABLE SDL2_CFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND sdl2-config --libs OUTPUT_VARIABLE SDL2_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

# Set the include and link directories based on sdl2-config output
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SDL2_CFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SDL2_LIBS}")

# Manually locate SDL2_image
find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h PATHS /opt/homebrew/include/SDL2 /usr/local/include/SDL2)
find_library(SDL2_IMAGE_LIBRARY NAMES SDL2_image PATHS /opt/homebrew/lib /usr/local/lib)

# Manually locate SDL2_mixer
find_path(SDL2_MIXER_INCLUDE_DIR SDL_mixer.h PATHS /opt/homebrew/include/SDL2 /usr/local/include/SDL2)
find_library(SDL2_MIXER_LIBRARY NAMES SDL2_mixer PATHS /opt/homebrew/lib /usr/local/lib)

# Manually locate SDL2_ttf
find_path(SDL2_TTF_INCLUDE_DIR SDL_ttf.h PATHS /opt/homebrew/include/SDL2 /usr/local/include/SDL2)
find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf PATHS /opt/homebrew/lib /usr/local/lib)

# Check if all required libraries are found
if(NOT SDL2_IMAGE_INCLUDE_DIR OR NOT SDL2_IMAGE_LIBRARY)
    message(FATAL_ERROR "SDL2_image not found. Please make sure SDL2_image is installed and the paths are correct.")
endif()

if(NOT SDL2_MIXER_INCLUDE_DIR OR NOT SDL2_MIXER_LIBRARY)
    message(FATAL_ERROR "SDL2_mixer not found. Please make sure SDL2_mixer is installed and the paths are correct.")
endif()

if(NOT SDL2_TTF_INCLUDE_DIR OR NOT SDL2_TTF_LIBRARY)
    message(FATAL_ERROR "SDL2_ttf not found. Please make sure SDL2_ttf is installed and the paths are correct.")
endif()

# Include directories for SDL2, SDL2_image, SDL2_mixer, and SDL2_ttf
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

# Link SDL2, SDL2_image, SDL2_mixer, and SDL2_ttf libraries
target_link_libraries(untitled ${SDL2_LIBS} ${SDL2_IMAGE_LIBRARY} ${SDL2_MIXER_LIBRARY} ${SDL2_TTF_LIBRARY})
