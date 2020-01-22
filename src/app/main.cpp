#include "mare/Application.hpp"
#include "mare/Scenes/SampleScene1.hpp"

// TODO:
// add widgets and UI
// Add more materials
// Create "Object" to hold meshes that will render with multiple materials
// finish input implementation in GLRenderer
// textures and framebuffers
// shadows and lights
// implement pop instance and others in the instanced mesh
// implement instanced mesh of instanced mesh rendering
// Add Text Thickness to CharMesh
// Phong material should render point light not directional light
// organize headers and cpp files, GL files should have protected inheritence from GLRenderer
// fix compiling with gcc on linux
// Cylinder, Cone, and Tube meshes should have an option for flat shading normals or smooth shading normals
// add exception handling

using namespace mare;

class Sandbox : public mare::Application
{
    Scene *scene_1;
    bool wireframe = false;

    void startup() override
    {
        set_window_title("Sandbox");
        scene_1 = new SampleScene1();
        set_scene(scene_1);
    }

    void render(double current_time, double delta_time) override
    {
    }

    void shutdown() override
    {
    }

    void on_key(const RendererInput &input) override
    {
        if (input.W_PRESSED)
        {
            wireframe = !wireframe;
        }
        wireframe_mode(wireframe);
    }
};

int main()
{
    mare::Application *app = new Sandbox();
    app->run(app);
    delete app;
    return 0;
}