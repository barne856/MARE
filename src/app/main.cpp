#include "mare/Application.hpp"

using namespace mare;

class Sandbox : public mare::Application
{
    glm::vec4 bg_color{237 / 255.0f, 201 / 255.0f, 175 / 255.0f, 1.0f};
    glm::vec4 tri_color{0.05f, 0.2f, 0.71f, 1.0f};
    Mesh *tri;
    BasicMaterial *basic_mat;

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
        basic_mat = GenBasicMaterial();
        basic_mat->set_color(tri_color);
        basic_mat->bind();
    }

    void render(double current_time, double delta_time) override
    {
        clear_color_buffer(bg_color);
        basic_mat->render();
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