// MARE
#include "mare/Scene.hpp"

namespace mare
{
Scene::Scene(ProjectionType type) : Layer(type) {}
Scene::~Scene() {}
void Scene::push_entity(Referenced<Entity> entity)
{
    for (auto entity_it = entity_begin(); entity_it != entity_end(); entity_it++)
    {
        if (*entity_it == entity)
        {
            return;
        }
    }
    entity_stack_.push_back(entity);
    return;
}

void Scene::pull_entity(Referenced<Entity> &entity)
{
    for (auto entity_it = entity_begin(); entity_it != entity_end(); entity_it++)
    {
        if (*entity_it == entity)
        {
            entity = *entity_it;
            return;
        }
    }
    return;
}

Referenced<Entity> Scene::pull_entity(size_t index)
{
    Referenced<Entity> pulled_entity = entity_stack_.at(index);
    entity_stack_.erase(entity_stack_.begin() + index);
    return pulled_entity;
}

Entity* Scene::get_entity(size_t index)
{
    return entity_stack_.at(index).get();
}

void Scene::push_overlay(Referenced<Overlay> overlay)
{
    for (auto overlay_it = overlay_begin(); overlay_it != overlay_end(); overlay_it++)
    {
        if (*overlay_it == overlay)
        {
            return;
        }
    }
    overlay_stack_.push_back(overlay);
}

void Scene::pop_overlay()
{
    overlay_stack_.pop_back();
}

Overlay* Scene::get_overlay(size_t index)
{
    return overlay_stack_.at(index).get();
}

} // namespace mare