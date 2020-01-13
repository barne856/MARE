#include "mare/Application.hpp"

// Renderers
#include "mare/GL/GLRenderer.hpp"

// Standard Library
#include <iostream>

// temp
#include "GLFW/glfw3.h"

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
    case RendererName::DirectX:
        std::cerr << "DirectX Rendering is not currently supported!" << std::endl;
        return;
    case RendererName::Vulkan:
        std::cerr << "Vulkan Rendering is not currently supported!" << std::endl;
        return;
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