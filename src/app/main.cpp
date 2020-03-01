#include "mare/GL/GLRenderer.hpp"
#include "mare/Scenes/SampleScene.hpp"

// TODO:

// GENERAL
// Specification of Renderer class which is run by main, Init and startup functions
// Scene stack with GenRef and load_scene

// MetaComponents for Scenes that control vectors of Entities? (Collision and shadows, multi entity systems)

// call mare::Initialize<GLRendererSpec>() to create static renderer object, access with Renderer::
// this will also run the GLrenderSpec

// SHADOWS & LIGHTS
// textures and framebuffers (fix simplemesh to add texture coordinates, set textures of materials, add repeating textures)
// material - point light, directional light, spotlight, hemisphere light, shadow mapping, texturing

// PHYSICS
// Physics component for transform feedback buffers GPU n body physics
// Particle system with transform feedback
// Cloth physics with transform feedback

// CLEANUP
// add exception handling
// fix compiling with gcc on linux
// DOCUMENTATION

// EXTRAS
// Cylinder, Cone, and Tube meshes should have an option for flat shading normals or smooth shading normals
// Add Text Thickness to CharMesh
// add bitmap fonts, maybe other formats too?
// add more widgets (buttons, switches, checkboxes, color picker, text entry, message box)
// Add more materials: normal maps and other physically based rendering techniques
// loaders for models?
// loaders sound? sound implimentation
// loaders for animations? animation implimentation

// OPTIMIZATION
// 1. Optimize CPU cache misses by reworking the ECS to have tightly packed data. Look into using bitfields to match entites with
// components and using matrix reordering to optimize the data when entites are added or removed.
// 2. Shaders should only have one instance for each kind which is shared by all of the materials that use it

using namespace mare;

int main()
{
    // Create the chosen API (make scoped)
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