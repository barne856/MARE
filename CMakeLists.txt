cmake_minimum_required(VERSION 3.15)

project(MARE)

add_definitions(-DGLEW_STATIC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Debug)

include_directories("./inc" "./ext/glfw/include" "./ext/glew-2.1.0/include" "./ext/glm/glm" "./ext/loaders/stb")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF)
set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG")

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
    target_link_libraries(APP GL glfw pthread X11 dl stdc++fs)
endif(UNIX)
# MSVC libraries
if(MSVC)
    target_link_libraries(APP opengl32 glfw)
endif(MSVC)