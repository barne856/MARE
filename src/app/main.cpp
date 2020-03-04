// TODO:

// SHADOWS & LIGHTS
// textures and framebuffers (add repeating textures), should be a type of buffer
// Buffers for point, directoinal, spot, and hemisphere lights
// Buffers for shadowmap framebuffers
// Meta RendererComponent that holds shadowmap buffers and light buffers in a Scene (MetaComponents can access every entity in the scene)
// Meta RendererComponent operates on every entity with corresponding component (eg. MetaLightComponent holds buffer of types of lights, torusRenderer has a lightComponent that sources lights from the metaComponent) 
//                                                                              (eg. MetaShadowComponent holds shadowbuffer, torusRenderer has a shadowComponent that will render into the depthbuffer of the metaComponent) 

// PHYSICS
// Physics component for transform feedback buffers GPU n body physics
// Particle system with transform feedback
// Cloth physics with transform feedback
// MetaComponent for collisions

// SCENES
// load different scenes

// CLEANUP
// add exception handling

// DOCUMENTATION
// Document the API

// EXTRAS
// Add teapot primative mesh
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

#include "mare/GL/GLRenderer.hpp"
#include "mare/Scenes/SampleScene.hpp"

class Sandbox : public mare::GLRenderer
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
        using namespace mare;
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
    return mare::launch<Sandbox>();
}