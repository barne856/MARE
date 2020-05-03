// TODO version 1:

// PHYSICS
// Make ShaderProgram class base class of GraphicsProgram and ComputeProgram
// Make Material base class of GraphicsProgram
// Move shader cahces to material and only compile each shader once an reuse across materials
// Compute Shaders (ComputeProgram class)
// Particle system
// Cloth physics
// MetaComponent for collisions

// CLEANUP
// make push entity a tempalte function for scenes
// add exception handling
// compile on linux with gcc
// Document the API

// TODO version 2:
// Beef up the Buffer, Texture, and Framebuffer classes to have all of the OpenGL features
// Beef-up Renderer blending to have all the opengl features like custum blend functions
// Bezier curves and spline curves / surfaces
// Add teapot primative mesh with bezier curves and surfaces
// Cylinder, Cone, and Tube meshes should have an option for flat shading normals or smooth shading normals
// Add Text Thickness to CharMesh
// Font Loader: bitmap fonts, maybe other formats too like vector fonts with ability to extrude text?
// Text billboards
// raypicking entities
// add more widgets (buttons, switches, checkboxes, color picker, text entry, message box)
// Add more materials (BasicColor, BasicTexture, Depth(Material for a scene in the scene's render component), Phong, Toon, Shadow material): normal maps and other physically based rendering techniques
// loaders for models?
// loaders sound? sound implimentation
// loaders for animations? animation implimentation

#include "mare/GL/GLRenderer.hpp"
#include "mare/Scenes/SampleScene.hpp"
#include "mare/Scenes/ComputeTextureScene.hpp"

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
        auto scene_1 = GenNamedRef<ComputeTextureScene>("Scene 1");
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