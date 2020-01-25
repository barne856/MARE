#include "mare/Renderer.hpp"
#include "mare/Layer.hpp"

namespace mare
{
RendererInfo Renderer::info{};
RendererInput Renderer::input{};
bool Renderer::running = false;
Application *Renderer::m_app_pointer = nullptr;
std::vector<Layer*>* Renderer::m_layer_stack = nullptr;

void Renderer::set_layer_stack(std::vector<Layer*>* layer_stack)
{
    m_layer_stack = layer_stack;
}

} // namespace mare
