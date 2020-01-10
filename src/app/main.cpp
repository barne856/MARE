#include "mare/Application.hpp"
#include "GL/glew.h"
#include "iostream"

class Sandbox : public mare::Application
{
    glm::vec4 bg_color {0.5, 0.0, 0.5, 1.0};

    void startup() override
    {
        
    }

    void render(double current_time, double delta_time) override
    {
        glClearBufferfv(GL_COLOR, 0, &bg_color[0]);
    }

    void shutdown() override
    {

    }

    void on_resize(int w, int h) override
    {
        get_info().window_width = w;
        get_info().window_height = h;
        get_info().window_aspect = float(get_info().window_width) / float(get_info().window_height);
        glViewport(0, 0, w, h);
    }
};

int main()
{
    mare::Application* app = new Sandbox();
    app->run(app);
    delete app;
    return 0;
}