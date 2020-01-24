#include "mare/Application.hpp"
#include "mare/Scenes/SampleScene1.hpp"

// TODO:
// add widgets objects and sample UI (objects are just a std::map of mesh material pairs with a render function, widgets are just layers)
// implement pop instance and others in the instanced mesh
// implement instanced mesh of instanced mesh rendering
// finish input implementation in GLRenderer
// shadows and lights
// textures and framebuffers
// Cylinder, Cone, and Tube meshes should have an option for flat shading normals or smooth shading normals
// Phong material should render point light not directional light
// organize headers and cpp files, GL files should have protected inheritence from GLRenderer
// fix compiling with gcc on linux
// Add Text Thickness to CharMesh
// Add more materials
// add exception handling

using namespace mare;

class Sandbox : public mare::Application
{
    std::vector<Layer*>* layer_stack;
    Layer* scene_1;

    void startup() override
    {
        set_window_title("Sandbox");
        scene_1 = new SampleScene1();
        layer_stack = new std::vector<Layer*>();
        layer_stack->push_back(scene_1);
        set_layer_stack(layer_stack);
    }

    void render(double current_time, double delta_time) override
    {
    }

    void shutdown() override
    {
    }


};

int main()
{
    mare::Application *app = new Sandbox();
    app->run(app);
    delete app;
    return 0;
}