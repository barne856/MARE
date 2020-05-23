#include "mare/Renderer.hpp"
#include "mare/Scene.hpp"

namespace mare {
// Static variables
RendererInfo Renderer::info{}; // Renderer and window information
RendererInput
    Renderer::input{}; // Inputs from keyboard and mouse and focued layer
bool Renderer::running{false};                      // Program is running?
Renderer *Renderer::API{nullptr};                   // The implemented API
std::vector<Referenced<Scene>> Renderer::scenes_{}; // the scene stack

// Static methods
void Renderer::end_renderer() { running = false; }
RendererInfo &Renderer::get_info() { return info; }
RendererInput &Renderer::get_input() { return input; }
void Renderer::load_scene(Scene *scene) {
  // If there is a current scene, exit scene
  if (info.scene) {
    for (auto layr_it = info.scene->layer_rbegin();
         layr_it != info.scene->layer_rend(); layr_it++) {
      Layer *layer = layr_it->get();
      layer->on_exit();
    }
    info.scene->on_exit();
  }
  // Replace the scene pointer
  info.scene = scene;
  // If the new scene is not nullptr, enter scene
  if (scene) {
    info.scene->on_enter();
    for (auto layr_it = info.scene->layer_begin();
         layr_it != info.scene->layer_end(); layr_it++) {
      Layer *layer = layr_it->get();
      layer->on_enter();
    }
  }
}
void Renderer::load_scene(int index) {
  // If there is a current scene, exit scene
  if (info.scene) {
    for (auto layr_it = info.scene->layer_rbegin();
         layr_it != info.scene->layer_rend(); layr_it++) {
      Layer *layer = layr_it->get();
      layer->on_exit();
    }
    info.scene->on_exit();
  }
  // Replace the scene pointer
  if (API) {
    info.scene = get_scenes<Scene>()[index];
  }
  // If the new scene is not nullptr, enter scene
  if (info.scene) {
    info.scene->on_enter();
    for (auto layr_it = info.scene->layer_begin();
         layr_it != info.scene->layer_end(); layr_it++) {
      Layer *layer = layr_it->get();
      layer->on_enter();
    }
  }
}
} // namespace mare
