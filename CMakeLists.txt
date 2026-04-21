cmake_minimum_required(VERSION 3.16)
project(NongEngineWorkspace CXX)

# Use modern C++
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# Automatically download and build SDL3 from GitHub
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG        main # Pulls the latest stable development branch
)
set(SDL_SHARED OFF CACHE BOOL "Build SDL shared library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build SDL static library" FORCE)
FetchContent_MakeAvailable(SDL3)

# Download stb
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(stb)

# ==============================================================================
# 0. COMPILE THE SHADERS
include(cmake/ShaderBaker.cmake)

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
    NONG/src/image.cpp
    NONG/src/texture.cpp    
    NONG/src/shader.cpp
    NONG/src/material.cpp
    NONG/src/mesh.cpp
    NONG/src/graphics_pipeline.cpp
    NONG/src/material.cpp
    NONG/src/renderer.cpp
    NONG/src/camera.cpp
    NONG/src/sprite_renderer.cpp
    NONG/src/render_texture.cpp
    NONG/src/texture2d.cpp
    ${GENERATED_CPP_FILE}
)
add_dependencies(NONG CompileShaders)

# Tell CMake where your engine's header files (NONG.h, component.h, etc.) live.
# PUBLIC means that any game linking to NONG will also know where these headers are.
target_include_directories(NONG PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/NONG/include)

# Link SDL3 and SDL Shader Cross to the engine.
# PUBLIC means that any game won't have to manually link SDL3; it inherits it through NONG.
target_link_libraries(NONG PUBLIC SDL3::SDL3)
# Link stb to the engine.
target_include_directories(NONG PUBLIC ${stb_SOURCE_DIR})

# ==============================================================================
# 2. BUILD THE TEST EXECUTABLE 
# ==============================================================================
# This current main.cpp used just for testing the engine.
add_executable(EngineTest main.cpp)

# Link your custom NONG engine to the test executable.
# PRIVATE means EngineTest uses NONG, but doesn't pass it on to anything else.
target_link_libraries(EngineTest PRIVATE NONG)