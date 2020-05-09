// TODO version 1:
// compile on linux with gcc
// Document the API
//  Each class should list every property and method with a description and example of usage
//  Renderer is special case for discussion, and functions in math, util and Mare.hpp are discussed separatly
//  gen_shader, gen_framebuffer, gen_texture*, gen_buffer
//  gen_scoped, gen_ref
//  assets, components, entities, overlays, widgets, scenes
//  Examples for GLRenderer spec, scene, overlay, entity, widget, mesh, material, component, graphics program, and compute program

// TODO version 2:
// Automated ShadowMap camera view creation
// Add Text Thickness to CharMesh
// Font Loader: bitmap fonts, maybe other formats too like vector fonts with ability to extrude text?
// Text billboards
// raypicking entities
// Cylinder, Cone, and Tube meshes should have an option for flat shading normals or smooth shading normals
// add more widgets (buttons, switches, checkboxes, color picker, text entry, message box)
// Add more materials (BasicColor, BasicTexture, Depth(Material for a scene in the scene's render component), Phong, Toon, Shadow material): normal maps and other physically based rendering techniques
// ComputeProgram memory barriers

// TODO version 3:
// Cloth physics
// MetaComponent for collisions
// Bezier curves and spline curves / surfaces
// Add teapot primative mesh with bezier curves and surfaces
// metaballs (marching cubes? quads with inverse distance shading?)
// loaders for models?
// loaders sound? sound implementation
// loaders for animations? animation implimentation
// batch rendering, convert composite meshes to meshes that can be rendered with gl multidraw calls
// Beefed-up Renderer blending to have all the opengl features like custom blend functions
// Beefed-up Buffer, Texture, and Framebuffer classes to have all of the OpenGL features

#include "mare/GL/GLRenderer.hpp"
#include "mare/Scenes/SampleScene.hpp"
#include "mare/Scenes/ComputeTextureScene.hpp"
#include "mare/Scenes/SampleParticleScene.hpp"
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
        gen_scene<SampleParticleScene>();
        load_scene(0);
    }
    void shutdown() override
    {
    }
};

int main(int argc, char ** argv)
{
    return mare::launch<Sandbox>();
}