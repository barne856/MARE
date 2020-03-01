#include "mare/Renderer.hpp"

namespace mare
{

// Static methods
void Renderer::end_renderer() { running = false; }
RendererInfo &Renderer::get_info() { return info; }
RendererInput &Renderer::get_input() { return input; }

// Static variables
RendererInfo Renderer::info{};    // Renderer and window information
RendererInput Renderer::input{};  // Inputs from keyboard and mouse and focued layer
bool Renderer::running{false};    // Program is running?
Renderer* Renderer::API{nullptr}; // The implemented API
} // namespace mare
