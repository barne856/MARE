cmake_minimum_required(VERSION 3.1)

project(APP)

add_definitions(-D_USE_MATH_DEFINES)
add_definitions(-DGLEW_STATIC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories("./inc" "./ext/glfw/include" "./ext/glew-2.1.0/include" "./ext/glm/glm")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(ext/glfw)

set(SRC
./ext/glew-2.1.0/src/glew.c
./src/app/main.cpp)

add_executable(APP ${SRC})
# UNIX libraries
if(UNIX)
    target_link_libraries(APP GL glfw pthread X11 dl stdc++fs)
endif(UNIX)
# MSVC libraries
if(MSVC)
    target_link_libraries(APP opengl32 glfw)
endif(MSVC)