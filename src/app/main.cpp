#include "mare/Application.hpp"

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
};

int main()
{
    mare::Application* app = new Sandbox();
    app->run(app);
    delete app;
    return 0;
}