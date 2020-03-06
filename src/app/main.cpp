// TODO:

// rename components to systems and metacomponents to metasystems

// Inherit from buffer -> Geometry buffer<float>(verts, norms, texcoords, also 'is a' render state(and render state should be API specific)), uniform buffers<T>, storage buffers<T>, texture buffer<unsigned char>, framebuffer buffer (inherits from texture buffer)
// Storage buffer has transform feedback option Feedback buffer enum in buffer type
// Transform feedback material, shadow material, applied on top of base material in entity's render component

// how to render in stages? First render transform feedback, then base material, then render depth to texture from light's view, then shadow material

// Texture2D
//         ID
//         set_buffer()
//         Buffer<Texture2D>(filepath)
// 
// Framebuffer
//         ID        
//         depth
//         color
//         set_buffer()
//         Buffer<Framebuffer>
//
// Textures: Min Filter and Mag filter, wrapU, wrapT, and repeat methods, gen mipmaps

// SHADOWS & LIGHTS
// textures and framebuffers (add repeating textures), should be a type of buffer
// Buffers for point, directoinal, spot, and hemisphere lights
// Buffers for shadowmap framebuffers
// Meta RendererComponent that holds shadowmap buffers and light buffers in a Scene (MetaComponents can access every entity in the scene)
// Meta RendererComponent operates on every entity with corresponding component (eg. MetaLightComponent holds buffer of types of lights, torusRenderer has a lightComponent that sources lights from the metaComponent, Requires material that reacts to lights) 
//                                                                              (eg. MetaShadowComponent holds shadowbuffer, torusRenderer has a shadowComponent(Material) that will render into the depthbuffer of the metaComponent) 

// PHYSICS
// Physics component for transform feedback buffers GPU n body physics (geometry buffer to geometery buffer feedback on GPU(can it be one double buffered geometry buffer?))
// Particle system with transform feedback
// Cloth physics with transform feedback
// MetaComponent for collisions

// CLEANUP
// add exception handling
// Vsync and cursor hidden should not be in input but should be their own functions
// Document the API

// EXTRAS
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

// OPTIMIZATION
// 1. Optimize CPU cache misses by reworking the ECS to have tightly packed data. Look into using bitfields to match entites with
// components and using matrix reordering to optimize the data when entites are added or removed.
// 2. Shaders should only have one instance for each kind which is shared by all of the materials that use it

// THE BAD STUFF
// slow entity-component system (difficult and possibly breaking change) (many entites will slow the renderer down fast)
// no animations (difficult but not breaking)
// no sound (easy fix)
// no external model loading (easy fix)
// no fancy rendering techniques like precomputed radiance transfer or physically based rendering (difficult but not breaking)

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