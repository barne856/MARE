#include "mare/GL/GLRenderer.hpp"
#include "mare/Scenes/SampleScene.hpp"

// TODO:

// Replace Application with Renderer completely, do not inherit from Renderer. Simply create a Renderer in main, set the settings, then add a scene network to it and run the renderer. 
// A scene network is a directed network of renderable scene, each scene can exit itself and load another level for which it has a pointer to
// Scenes should separate render and logic operations
// Initially, create the root scene set it as the renderer's Scene. This will add it to the cache of created scenes
// When a Scene wants to change to another scene of a certain type, check if a cached scene is availbale or else create that scene and add set the renderer's scene to it.

// CLEANUP
// organize headers and cpp files, GL files should have protected inheritence from GLRenderer
// replace pointers with smart pointers
// implement pop instance and others in the instanced mesh
// fix compiling with gcc on linux
// add exception handling
// finish input implementation in GLRenderer
// screen to world coordinates with 3D
// Free flying camera controls
// DOCUMENTATION

// SHADOWS & LIGHTS
// textures and framebuffers (fix simplemesh to add texture coordinates, set textures of materials, add repeating textures)
// material - point light, directional light, spotlight, hemisphere light, shadow mapping, texturing

// ADVANCE DESIGN
// Components for objects
// review best practices and game engine patterns

// EXTRAS
// Cylinder, Cone, and Tube meshes should have an option for flat shading normals or smooth shading normals
// Add Text Thickness to CharMesh
// add bitmap fonts, maybe other formats too?
// add more widgets (buttons, switches, checkboxes, color picker, text entry, message box)
// Add more materials: normal maps and other physically based rendering techniques

using namespace mare;

int main()
{
    Renderer *OpenGL = new GLRenderer();
    Renderer::API = OpenGL;
    SampleScene *scene_1 = new SampleScene();
    OpenGL->get_info().scene = scene_1;
    OpenGL->get_info().window_title = "Sandbox";
    Renderer::run();
    delete OpenGL;
    delete scene_1;
    return 0;
}