cmake_minimum_required(VERSION 3.1)

project(APP)

add_definitions(-DGLEW_STATIC)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories("./inc" "./ext/glfw/include" "./ext/glew-2.1.0/include" "./ext/glm/glm" "./ext/loaders/stb")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(ext/glfw)

set(SRC
./ext/glew-2.1.0/src/glew.c
./src/app/main.cpp
./src/mare/Buffer.cpp
./src/mare/Camera.cpp
./src/mare/CompositeMesh.cpp
./src/mare/Controls.cpp
./src/mare/InstancedMesh.cpp
./src/mare/MareUtility.cpp
./src/mare/Material.cpp
./src/mare/Mesh.cpp
./src/mare/Object.cpp
./src/mare/Overlay.cpp
./src/mare/Renderer.cpp
./src/mare/RenderState.cpp
./src/mare/Scene.cpp
./src/mare/SimpleMesh.cpp
./src/mare/Widget.cpp
./src/mare/GL/GLBuffer.cpp
./src/mare/GL/GLRenderer.cpp
./src/mare/GL/GLRenderState.cpp
./src/mare/GL/GLShader.cpp
./src/mare/GL/GLTexture.cpp)

add_executable(APP ${SRC})
# UNIX libraries
if(UNIX)
    target_link_libraries(APP GL glfw pthread X11 dl stdc++fs)
endif(UNIX)
# MSVC libraries
if(MSVC)
    target_link_libraries(APP opengl32 glfw)
endif(MSVC)