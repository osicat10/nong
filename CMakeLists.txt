cmake_minimum_required(VERSION 3.16)
project(NongEngineWorkspace CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Automatically download and build SDL3 from GitHub
include(FetchContent)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        main # Pulls the latest stable development branch
)
set(SDL_SHARED OFF CACHE BOOL "Build SDL shared library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build SDL static library" FORCE)
FetchContent_MakeAvailable(SDL3)

# ==============================================================================
# 1. BUILD THE ENGINE LIBRARY (NONG)
# ==============================================================================
# We create a STATIC library containing all your engine source files.
# Note: Adjust the file paths below if your files are inside a specific folder!
add_library(NONG STATIC 
    NONG/src/component.cpp
    NONG/src/object.cpp
    NONG/src/vector.cpp
    NONG/src/input.cpp
    NONG/src/color.cpp
    NONG/src/window.cpp
    NONG/src/time.cpp
)

# Tell CMake where your engine's header files (NONG.h, component.h, etc.) live.
# PUBLIC means that any game linking to NONG will also know where these headers are.
target_include_directories(NONG PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/NONG/include)

# Link SDL3 to the engine.
# PUBLIC means that any game won't have to manually link SDL3; it inherits it through NONG.
target_link_libraries(NONG PUBLIC SDL3::SDL3)

# ==============================================================================
# 2. BUILD THE TEST EXECUTABLE 
# ==============================================================================
# This current main.cpp used just for testing the engine.
add_executable(EngineTest main.cpp)

# Link your custom NONG engine to the test executable.
# PRIVATE means EngineTest uses NONG, but doesn't pass it on to anything else.
target_link_libraries(EngineTest PRIVATE NONG)