// MARE
#include "mare/Scene.hpp"

namespace mare
{
Scene::Scene() {}
Scene::~Scene() {}
shader_data_type Scene::get_widget_value(size_t overlay_index, size_t widget_index)
{
    return get_overlay(overlay_index)->get_widget(widget_index)->get_value();
}
Overlay *Scene::get_overlay(size_t index)
{
    // the 0 index is always the base layer "this"
    return overlay_stack_[index].get();
}
void Scene::push_overlay(Scoped<Overlay> overlay)
{
    overlay_stack_.push_back(std::move(overlay));
}

std::vector<Scoped<Overlay>>::const_iterator Scene::begin()
{
    return overlay_stack_.begin();
}
std::vector<Scoped<Overlay>>::const_iterator Scene::end()
{
    return overlay_stack_.end();
}

std::vector<Scoped<Overlay>>::const_reverse_iterator Scene::rbegin()
{
    return overlay_stack_.rbegin();
}
std::vector<Scoped<Overlay>>::const_reverse_iterator Scene::rend()
{
    return overlay_stack_.rend();
}

} // namespace mare