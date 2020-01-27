#include "mare/Application.hpp"
#include "mare/Scenes/SampleScene.hpp"

// TODO:
// textures and framebuffers (fix simplemesh to add texture coordinates, set textures of materials, add repeating textures)
// shadows and lights
// Phong material should render point light not directional light
// Add more materials
//
// finish input implementation in GLRenderer
// Cylinder, Cone, and Tube meshes should have an option for flat shading normals or smooth shading normals
// Add Text Thickness to CharMesh
// add more widgets
// implement pop instance and others in the instanced mesh
// replace pointers with smart pointers
// organize headers and cpp files, GL files should have protected inheritence from GLRenderer
// fix compiling with gcc on linux
// add exception handling

using namespace mare;

class Sandbox : public mare::Application
{
    SampleScene *scene_1;
    glm::vec4 bg_color{0.0f, 0.0f, 0.0f, 1.0f};

    void startup() override
    {
        set_window_title("Sandbox");
        scene_1 = new SampleScene();
        set_scene(scene_1);
        enable_blending(true);
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