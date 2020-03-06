#include "mare/Renderer.hpp"
#include "mare/Scene.hpp"

namespace mare
{
// Static variables
RendererInfo Renderer::info{};    // Renderer and window information
RendererInput Renderer::input{};  // Inputs from keyboard and mouse and focued layer
bool Renderer::running{false};    // Program is running?
Renderer *Renderer::API{nullptr}; // The implemented API

// Static methods
void Renderer::end_renderer() { running = false; }
RendererInfo &Renderer::get_info() { return info; }
RendererInput &Renderer::get_input() { return input; }
void Renderer::load_scene(Scene *scene)
{
    // If there is a current scene, exit scene
    if (info.scene)
    {
        info.scene->on_exit();
    }
    // Replace the scene pointer
    info.scene = scene;
    // If the new scene is not nullptr, enter scene
    if (scene)
    {
        info.scene->on_enter();
    }
}
void Renderer::load_scene(std::string name)
{
    // If there is a current scene, exit scene
    if (info.scene)
    {
        info.scene->on_exit();
    }
    // Replace the scene pointer, nullptr if "name" is not the name of a scene
    if (API)
    {
        info.scene = API->GetRefByName<Scene>(name).get();
    }
    // If the new scene is not nullptr, enter scene
    if (info.scene)
    {
        info.scene->on_enter();
    }
}
} // namespace mare
