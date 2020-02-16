#include "mare/GL/GLRenderer.hpp"
#include "mare/Scenes/SampleScene.hpp"

// TODO:

// CLEANUP
// screen to world coordinates with 3D
// replace pointers with smart pointers
// add exception handling
// fix compiling with gcc on linux
// DOCUMENTATION

// Create a scene network which is a directed network of renderable scenes, each scene can exit itself and load another level for which it has a pointer to
// Scenes should separate render and logic operations
// Initially, create the root scene set it as the renderer's Scene. This will add it to the cache of created scenes
// When a Scene wants to change to another scene of a certain type, check if a cached scene is availbale or else create that scene and add set the renderer's scene to it.

// SHADOWS & LIGHTS
// textures and framebuffers (fix simplemesh to add texture coordinates, set textures of materials, add repeating textures)
// material - point light, directional light, spotlight, hemisphere light, shadow mapping, texturing

// Particle system with transform feedback
// Cloth physics with transform feedback

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
    // Create the chosen API
    Renderer *OpenGL = new GLRenderer();
    // Set renderer settings
    OpenGL->get_info().samples = 16;
    OpenGL->get_info().window_title = "Sandbox";
    // Initialize the chosen API
    OpenGL->init();
    // Set the active API
    Renderer::API = OpenGL;
    // Create the root scene and set it to active
    SampleScene *scene_1 = new SampleScene();
    OpenGL->get_info().scene = scene_1;
    // Run the application
    Renderer::run();
    // Cleanup
    delete OpenGL;
    delete scene_1;
    // Return to the operating system
    return 0;
}