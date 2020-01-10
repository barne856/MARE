#include "mare/Application.hpp"

// Renderers
#include "mare/GLRenderer.hpp"

namespace mare
{

// Application run function
void Application::run(Application *app_pointer)
{
    init(Renderer::get_info());
    switch (Renderer::get_info().name)
    {
        case RendererName::OpenGL:
            m_renderer = new GLRenderer();
            break;
        default:
            m_renderer = new GLRenderer();
    }
    m_renderer->renderer_process(app_pointer);
    delete m_renderer;
}

// Initialize static members
Application *Application::m_app_pointer = nullptr;
Renderer *Application::m_renderer = nullptr;
} // namespace mare