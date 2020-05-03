cmake_minimum_required(VERSION 3.1)

project(APP)

add_definitions(-DGLEW_STATIC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

include_directories("./inc" "./ext/glfw/include" "./ext/glew-2.1.0/include" "./ext/glm/glm" "./ext/loaders/stb")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(ext/glfw)

set(SRC
./ext/glew-2.1.0/src/glew.c
./src/app/main.cpp
./src/mare/Buffers.cpp
./src/mare/Mare.cpp
./src/mare/Meshes.cpp
./src/mare/Overlay.cpp
./src/mare/Renderer.cpp
./src/mare/Scene.cpp
./src/mare/Shader.cpp
./src/mare/Transform.cpp
./src/mare/Widget.cpp
./src/mare/GL/GLBuffers.cpp
./src/mare/GL/GLRenderer.cpp
./src/mare/GL/GLShader.cpp)

add_executable(APP ${SRC})
# UNIX libraries
if(UNIX)
    target_link_libraries(APP GL glfw pthread X11 dl stdc++fs)
endif(UNIX)
# MSVC libraries
if(MSVC)
    target_link_libraries(APP opengl32 glfw)
endif(MSVC)