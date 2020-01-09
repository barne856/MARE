# MARE
Minimal abstraction rendering engine using glfw, glew, OpenGL 4.5, and C++17

## Dependencies & External Libraries:
    GLFW3 (git submodule)
    GLEW  (included in project)
    OpenGL 4.5 Capable Hardware
    C++17 Compiler (Visual Studio 2019 MSVC tested)

## Clone with submodules:
git clone --recurse-submodules -j8 https://github.com/barne856/MARE.git

## Build with CMake (from root directory):
    mkdir build
    cd build
    cmake ..
    cd ..
    cmake --build ./build