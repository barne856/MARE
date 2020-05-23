// TODO version 1:
// Document the API
//  Examples for GLRenderer spec, scene, layer, entity, widget, mesh, material,
// component, graphics program, and compute program in README

// TODO version 2:
// Phong material should ahve arrays for spotlight, point light, and directional lights, as well as shadowmaping for directional, point, and spot lights
// shadowmap should have arrays for each type of light and depth buffers/light views for each
// Automated ShadowMap camera view creation
// Add just_released to renderer input
// Add Text Thickness to CharMesh using quads and circles
// Font Loader: bitmap fonts, maybe other formats too like vector fonts with ability to extrude text?
// Text billboards
// raypicking entities
// Cylinder, Cone, and Tube meshes should have an option for flat shading normals or smooth shading normals
// add more widgets (buttons, switches, checkboxes, color picker, text entry, message box)
// Add more materials (BasicColor, BasicTexture, Depth(Material for a scene in the scene's render component), Phong, Toon, Shadow material): normal maps and other physically based rendering techniques

// TODO version 3:
// InstancedMeshes have a TransformBuffer not a glm::mat4 Buffer. Transform Component should only contain a single glm::mat4 with functions to get and set things.
// api_render_mesh should use Transforms to render not glm::mat4
// MetaComponent for collisions
// Bezier curves and spline curves / surfaces
// Add teapot primative mesh with bezier curves and surfaces
// Cloth physics
// metaballs (marching cubes? quads with inverse distance shading?)
// loaders for models?
// loaders sound? sound implementation
// loaders for animations? animation implimentation
// batch rendering, convert composite meshes to meshes that can be rendered with gl multidraw calls
// Beefed-up Renderer blending to have all the opengl features like custom blend functions
// Beefed-up Buffer, Texture, and Framebuffer classes to have all of the OpenGL features

#include "mare/GL/GLRenderer.hpp"
#include "app/Scenes/SampleShadowScene.hpp"
#include "app/Scenes/SampleParticleScene.hpp"

class Sandbox : public mare::GLRenderer
{
public:
    void init_info() override
    {
        info.window_title = "Sandbox";     // Window title
        info.window_width = 1280;          // window width in pixels
        info.window_height = 720;          // window height in pixels
        info.window_aspect = 16.0f / 9.0f; // window aspect ratio
        info.samples = 8;                 // antialiasing samples
        info.fullscreen = false;           // render in fullscreen mode?
        info.vsync = true;                // render in double buffered vsync mode?
        info.debug_mode = 1;               // 0000 == off, 0001 == high, 0010 == med, 0100 == low, 1000 == notification
    }
    void startup() override
    {
        using namespace mare;
        auto layer1 = gen_scene<SampleShadowScene>();
        auto layer2 = gen_scene<SampleParticleScene>();
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