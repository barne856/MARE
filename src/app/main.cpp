#include "mare/GL/GLRenderer.hpp"
#include "mare/Scenes/SampleScene.hpp"

// TODO:

// CLEANUP
// Generalize controls to work on any gameobject not just cameras
// Specification of Renderer class which is run by main
// Scene stack with GenRef and load_scene
// separation of logic from rendering. create components for objects with components for rendering, physics, and callbacks
// add exception handling
// fix compiling with gcc on linux
// DOCUMENTATION

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