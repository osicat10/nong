# NONG Engine Prototype

A lightweight, cross-platform C++ game engine built from scratch. NONG utilizes an Entity-Component System (ECS) architecture and handles low-level graphics/compute abstraction via SDL3's modern GPU API.

Currently, this repository serves as the core engine library and includes an `EngineTest` executable for rapid prototyping.

## 🚀 Features
* **Custom Entity-Component Architecture:** Safely manages memory and object lifecycles using modern C++ smart pointers and macros.
* **Modern Graphics API:** Built on SDL3's generic GPU API, abstracting Vulkan, DirectX 12, and Metal without the boilerplate.
* **Modular Design:** Compiled as a static library (`.lib`/`.a`) designed to be easily included in future standalone game projects (like PONG).

## 🛠️ Prerequisites
Before building, ensure you have the following installed:
* A **C++23** compatible compiler (GCC/MinGW, Clang, or MSVC)
* **CMake** (version 3.16 or higher)
* **Git** (Required for CMake FetchContent to download SDL3)

## 🏗️ How to Build
This project uses CMake to automatically fetch dependencies (SDL3, SDL Shadercross and stb) and generate the build files.

1. Clone the repository:
   ```bash
   git clone https://github.com/osicat10/NONG.git
   cd NONG
   ```

2. Generate the build files:
   ```bash
   cmake -S . -B build
   ```

3. Compile the engine and test executable:
   ```bash
   cmake --build build
   ```

## 🎮 Running the Test
Once built, you can find the test executable inside the ```build``` folder.
Run ```./build/EngineTest``` (or ```EngineTest.exe``` on Windows) to launch the engine window.

## 📂 Project Structure
The engine is structured as a standard C++ library, strictly separating the public API from the private implementation.

* `NONG/include/NONG/` - **Public API.** Contains all header files (`.h`) and template implementations (`.tpp`). Any game linking to NONG will have access to these files via `#include "NONG/..."`.
* `NONG/src/` - **Private Implementation.** Contains the source code (`.cpp`) and private headers. These are compiled into the library and remain completely hidden from the end-user.
* `main.cpp` - The test executable that links to the NONG library.
* `CMakeLists.txt` - The build configuration.