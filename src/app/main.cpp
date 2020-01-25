#include "mare/Application.hpp"
#include "mare/Scenes/SampleScene.hpp"

// TODO:
// implement pop instance and others in the instanced mesh
// implement instanced mesh of instanced mesh rendering
// finish input implementation in GLRenderer
// shadows and lights
// textures and framebuffers
// Cylinder, Cone, and Tube meshes should have an option for flat shading normals or smooth shading normals
// Phong material should render point light not directional light
// organize headers and cpp files, GL files should have protected inheritence from GLRenderer
// replace pointers with smart pointers
// fix compiling with gcc on linux
// Add Text Thickness to CharMesh
// Add more materials
// add exception handling

using namespace mare;

class Sandbox : public mare::Application
{
    SampleScene *scene_1;
    glm::vec4 bg_color{0.1f, 0.08f, 0.12f, 1.0f};

    void startup() override
    {
        set_window_title("Sandbox");
        scene_1 = new SampleScene();
        set_scene(scene_1);
    }

    void render(double current_time, double delta_time) override
    {
        // Clear color and depth buffer
        Application::clear_color_buffer(bg_color);
        Application::clear_depth_buffer();
    }

    void shutdown() override
    {
        delete scene_1;
    }
};

int main()
{
    mare::Application *app = new Sandbox();
    app->run(app);
    delete app;
    return 0;
}