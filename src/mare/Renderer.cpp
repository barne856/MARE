#include "mare/Renderer.hpp"
namespace mare
{
RendererInfo Renderer::info{};
RendererInput Renderer::input{};
bool Renderer::running = false;
Application *Renderer::m_app_pointer = nullptr;
Camera* Renderer::m_camera_pointer = nullptr;
} // namespace mare
