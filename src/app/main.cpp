#include "mare/Application.hpp"
using namespace mare;

class Sandbox : public mare::Application
{
    glm::vec4 bg_color{237 / 255.0f, 201 / 255.0f, 175 / 255.0f, 1.0f};

    void init(RendererInfo &info) override
    {
        info.name = RendererName::OpenGL;
        info.window_width = 1280;
        info.window_height = 720;
        info.window_aspect = 1280.0f / 720.0f;
        info.major_version = 4;
        info.minor_version = 5;
        info.samples = 0;
        info.fullscreen = false;
        info.vsync = false;
        info.cursor = true;
        info.debug_mode = 1;
    }

    void startup() override
    {
        set_window_title("Sandbox");
    }

    void render(double current_time, double delta_time) override
    {
        clear_color_buffer(bg_color);
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