cmake_minimum_required(VERSION 3.28)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

# Add SDL2 include path to compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/opt/homebrew/include/SDL2")

# Find cJSON
find_path(CJSON_INCLUDE_DIR cJSON.h PATHS /opt/homebrew/include/cjson /usr/local/include/cjson /opt/homebrew/opt/cjson/include/cjson)
find_library(CJSON_LIBRARY NAMES cjson PATHS /opt/homebrew/lib /usr/local/lib /opt/homebrew/opt/cjson/lib)

if(NOT CJSON_INCLUDE_DIR OR NOT CJSON_LIBRARY)
    message(FATAL_ERROR "cJSON not found. Please make sure cJSON is installed and the paths are correct.")
endif()

# Add include directories
include_directories(${CJSON_INCLUDE_DIR})

add_executable(untitled main.cpp
        include/manager/manager.h
        include/manager/game_manager.h
        include/tile.h
        include/map.h
        include/route.h
        include/manager/config_manager.h
        include/enemy/enemy_type.h
        include/enemy/enemy_type.h
        include/wave.h)

# Find SDL2
find_path(SDL2_INCLUDE_DIR SDL.h PATHS /opt/homebrew/include/SDL2 /usr/local/include/SDL2)
find_library(SDL2_LIBRARY NAMES SDL2 PATHS /opt/homebrew/lib /usr/local/lib)

# Find SDL2_image
find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h PATHS /opt/homebrew/include/SDL2 /usr/local/include/SDL2)
find_library(SDL2_IMAGE_LIBRARY NAMES SDL2_image PATHS /opt/homebrew/lib /usr/local/lib)

# Find SDL2_mixer
find_path(SDL2_MIXER_INCLUDE_DIR SDL_mixer.h PATHS /opt/homebrew/include/SDL2 /usr/local/include/SDL2)
find_library(SDL2_MIXER_LIBRARY NAMES SDL2_mixer PATHS /opt/homebrew/lib /usr/local/lib)

# Find SDL2_ttf
find_path(SDL2_TTF_INCLUDE_DIR SDL_ttf.h PATHS /opt/homebrew/include/SDL2 /usr/local/include/SDL2)
find_library(SDL2_TTF_LIBRARY NAMES SDL2_ttf PATHS /opt/homebrew/lib /usr/local/lib)

# Check if all required libraries are found
if(NOT SDL2_INCLUDE_DIR OR NOT SDL2_LIBRARY)
    message(FATAL_ERROR "SDL2 not found. Please make sure SDL2 is installed and the paths are correct.")
endif()

if(NOT SDL2_IMAGE_INCLUDE_DIR OR NOT SDL2_IMAGE_LIBRARY)
    message(FATAL_ERROR "SDL2_image not found. Please make sure SDL2_image is installed and the paths are correct.")
endif()

if(NOT SDL2_MIXER_INCLUDE_DIR OR NOT SDL2_MIXER_LIBRARY)
    message(FATAL_ERROR "SDL2_mixer not found. Please make sure SDL2_mixer is installed and the paths are correct.")
endif()

if(NOT SDL2_TTF_INCLUDE_DIR OR NOT SDL2_TTF_LIBRARY)
    message(FATAL_ERROR "SDL2_ttf not found. Please make sure SDL2_ttf is installed and the paths are correct.")
endif()

# Link libraries
target_link_libraries(untitled ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_MIXER_LIBRARY} ${SDL2_TTF_LIBRARY} ${CJSON_LIBRARY})
