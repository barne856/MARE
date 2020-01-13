#include "mare/Application.hpp"

#include <iostream>

// TODO:
// Scenes to render material-mesh pairs with a camera
// implement composite mesh and instanced mesh
// add more primative meshes and Text mesh
// add more standard materials, create general material similar to ArrayMesh for Meshes
// add widgets and UI
// textures and framebuffers

using namespace mare;

class Sandbox : public mare::Application
{
    glm::vec4 bg_color{237 / 255.0f, 201 / 255.0f, 175 / 255.0f, 1.0f};
    glm::vec4 tri_color{0.05f, 0.72f, 0.71f, 1.0f};
    Mesh *tri;
    Material *basic_mat;
    Camera *main_camera;

    void init(RendererInfo &info) override
    {
        info.name = RendererName::OpenGL;
        info.window_width = 1280;
        info.window_height = 720;
        info.window_aspect = 1280.0f / 720.0f;
        info.major_version = 4;
        info.minor_version = 5;
        info.samples = 16;
        info.fullscreen = false;
        info.vsync = false;
        info.cursor = true;
        info.debug_mode = 1;
    }

    void startup() override
    {
        set_window_title("Sandbox");
        tri = GenTriangle({-0.5f, -0.5f}, {0.5f, -0.5f}, {0.5f, 0.5f});
        tri->size(glm::vec3(0.25f));
        basic_mat = GenMaterial("./res/Shaders/Basic");
        basic_mat->bind();
        main_camera = new Camera(CameraType::PERSPECTIVE);
        main_camera->set_controls(ControlsConfig::ORBITCONTROLS);
        main_camera->set_position(glm::vec3(0.0f, 0.0f, 1.0f));
        set_camera(main_camera);
    }

    void render(double current_time, double delta_time) override
    {
        clear_color_buffer(bg_color);
        basic_mat->upload_vec4("u_color", tri_color);
        basic_mat->upload_mat4("projection", main_camera->projection());
        basic_mat->upload_mat4("view", main_camera->view());
        basic_mat->upload_mat4("model", tri->get_model());
        tri->render();
    }

    void shutdown() override
    {
        // cleanup mesh and material
        delete tri;
        delete basic_mat;
    }
};

int main()
{
    mare::Application *app = new Sandbox();
    app->run(app);
    delete app;
    return 0;
}