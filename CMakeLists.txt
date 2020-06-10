cmake_minimum_required(VERSION 3.15)

project(MARE)

add_definitions(-DGLEW_STATIC)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(ext/glfw)

set(SRC
./ext/glew-2.1.0/src/glew.c
./src/Buffers.cpp
./src/Mare.cpp
./src/Meshes.cpp
./src/Renderer.cpp
./src/Shader.cpp
./src/GL/GLBuffers.cpp
./src/GL/GLRenderer.cpp
./src/GL/GLShader.cpp)

add_library(MARE STATIC ${SRC})
# UNIX libraries
if(UNIX)
    target_link_libraries(MARE GL glfw pthread X11 dl stdc++fs)
endif(UNIX)
# MSVC libraries
if(MSVC)
    target_link_libraries(MARE opengl32 glfw)
endif(MSVC)