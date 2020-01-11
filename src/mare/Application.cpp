#include "mare/Application.hpp"

// Renderers
#include "mare/GL/GLRenderer.hpp"

namespace mare
{

// Application run function
void Application::run(Application *app_pointer)
{
    init(Renderer::get_info());
    switch (Renderer::get_info().name)
    {
        case RendererName::OpenGL:
            m_renderer_pointer = new GLRenderer();
            break;
        default:
            m_renderer_pointer = new GLRenderer();
    }
    m_renderer_pointer->start_process(app_pointer);
    delete m_renderer_pointer;
}

// Initialize static members
Application *Application::m_app_pointer = nullptr;
Renderer *Application::m_renderer_pointer = nullptr;
} // namespace mare