#include "mare/GL/GLRenderer.hpp"
#include "mare/Scenes/SampleScene.hpp"

// TODO:

// SHADOWS & LIGHTS
// textures and framebuffers (fix simplemesh to add texture coordinates, set textures of materials, add repeating textures)
// material - point light, directional light, spotlight, hemisphere light, shadow mapping, texturing
// MetaComponents for Scenes that control vectors of Entities? (Collision and shadows, multi entity systems)

// PHYSICS
// Physics component for transform feedback buffers GPU n body physics
// Particle system with transform feedback
// Cloth physics with transform feedback

// SCENES
// load different scenes

// CLEANUP
// add exception handling
// fix compiling with gcc on linux

// DOCUMENTATION
// Document the API

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

class Sandbox : public GLRenderer
{
public:
    void init_info() override
    {
        info.window_title = "Sandbox";     // Window title
        info.window_width = 1280;          // window width in pixels
        info.window_height = 720;          // window height in pixels
        info.window_aspect = 16.0f / 9.0f; // window aspect ratio
        info.samples = 16;                 // antialiasing samples
        info.fullscreen = false;           // render in fullscreen mode?
        info.vsync = false;                // render in double buffered vsync mode?
        info.debug_mode = 1;               // 0000 == off, 0001 == high, 0010 == med, 0100 == low, 1000 == notification
    }
    void startup() override
    {
        auto scene_1 = GenNamedRef<SampleScene>("Scene 1");
        info.scene = scene_1.get();
    }
    void shutdown() override
    {
        DeleteRef("Scene 1");
    }
};

int main()
{
    return launch<Sandbox>();
}