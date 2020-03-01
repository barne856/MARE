#ifndef ENTITY
#define ENTITY

// MARE
#include "mare/MareUtility.hpp"
#include "mare/Transform.hpp"
#include "mare/Renderer.hpp"

namespace mare
{

class Entity : public Transform
{
public:
    Entity() {}
    virtual ~Entity() {}
    template <typename T>
    void push_component()
    {
        Referenced<Component> comp = std::dynamic_pointer_cast<Component>(Renderer::API->GenRef<T>());
        if (comp)
        {
            for(auto& component : components_)
            {
                if(std::dynamic_pointer_cast<T>(component))
                {
                    return;
                }
            }
            components_.push_back(comp);
        }
    }
    template <typename T>
    T *get_component()
    {
        for (auto comp_it = component_begin(); comp_it != component_end(); comp_it++)
        {
            if (auto component = std::dynamic_pointer_cast<T>(*comp_it))
            {
                return component.get();
            }
        }
        return nullptr;
    }
    template <typename T>
    std::vector<T *> get_components()
    {
        std::vector<T *> components{};
        for (auto comp_it = component_begin(); comp_it != component_end(); comp_it++)
        {
            if (auto component = std::dynamic_pointer_cast<T>(*comp_it))
            {
                components.push_back(component.get());
            }
        }
        return components;
    }
    template <typename T>
    Referenced<T> pull_component()
    {
        for (auto comp_it = component_begin(); comp_it != component_end(); comp_it++)
        {
            if (auto component = std::dynamic_pointer_cast<T>(*comp_it))
            {
                Referenced<T> pulled_component = std::move(component);
                components_.erase(comp_it);
                return pulled_component;
            }
        }
        return nullptr;
    }
    std::vector<Referenced<Component>>::const_iterator component_begin() const { return components_.begin(); }
    std::vector<Referenced<Component>>::const_iterator component_end() const { return components_.end(); }
    std::vector<Referenced<Component>>::const_reverse_iterator component_rbegin() const { return components_.rbegin(); }
    std::vector<Referenced<Component>>::const_reverse_iterator component_rend() const { return components_.rend(); }
    std::vector<Referenced<Component>>::iterator component_begin() { return components_.begin(); }
    std::vector<Referenced<Component>>::iterator component_end() { return components_.end(); }
    std::vector<Referenced<Component>>::reverse_iterator component_rbegin() { return components_.rbegin(); }
    std::vector<Referenced<Component>>::reverse_iterator component_rend() { return components_.rend(); }

private:
    std::vector<Referenced<Component>> components_;
};
} // namespace mare

#endif