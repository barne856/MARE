// TODO version 1:

// SHADOWS & LIGHTS
// Render in Stages: Compute Shader -> Material -> Shadows and Lights
// MetaRenderSystem that holds shadowmap textures, framebuffers, and light buffers in a Scene (MetaSystems can access every entity in the scene)
// MetaRenderSystem operates on every entity with corresponding component (eg. MetaLightComponent holds buffer of types of lights, torusRenderer has a lightComponent that sources lights from the metaComponent, Requires material that reacts to lights) 
//                                                                              (eg. MetaShadowComponent holds shadowbuffer, torusRenderer has a shadowComponent(Material) that will render into the depthbuffer of the metaComponent) 

// PHYSICS
// Compute Shaders
// Particle system with transform feedback
// Cloth physics with transform feedback
// MetaComponent for collisions

// CLEANUP
// add exception handling
// Vsync and cursor hidden should not be in input but should be their own functions
// Document the API

// TODO version 2:
// Beef up the Buffer, Texture, and Framebuffer classes to have all of the OpenGL features
// Beef-up Renderer blending to have all the opengl features like custum blend functions
// Add teapot primative mesh
// Cylinder, Cone, and Tube meshes should have an option for flat shading normals or smooth shading normals
// Add Text Thickness to CharMesh
// Font Loader: bitmap fonts, maybe other formats too like vector fonts with ability to extrude text?
// Bezier curves and spline curves / surfaces
// raypicking entities
// add more widgets (buttons, switches, checkboxes, color picker, text entry, message box)
// Add more materials (Basic, Depth(Material for a scene in the scene's render component), Phong, Toon, Shadow material): normal maps and other physically based rendering techniques
// loaders for models?
// loaders sound? sound implimentation
// loaders for animations? animation implimentation

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
        load_scene("Scene 1");
    }
    void shutdown() override
    {
        DeleteRef("Scene 1");
    }
};

int main(int argc, char ** argv)
{
    return mare::launch<Sandbox>();
}