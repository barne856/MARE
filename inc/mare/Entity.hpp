#ifndef ENTITY
#define ENTITY

// MARE
#include "mare/Mare.hpp"
#include "mare/Transform.hpp"
#include "mare/Renderer.hpp"

namespace mare
{

class Entity : public Transform
{
public:
    Entity() {}
    virtual ~Entity() {}
    // push a component onto the stack
    template <typename T, typename... Args>
    void push_component(Args... args)
    {
        Referenced<Component> comp = std::dynamic_pointer_cast<Component>(Renderer::API->GenRef<T>(args...));
        if (comp)
        {
            for (auto &component : components_)
            {
                // do not add component if the same concrete type already exists in the stack
                if (std::dynamic_pointer_cast<T>(component))
                {
                    return;
                }
            }
            components_.push_back(comp);
        }
        // nothing pushed if T is not a component
    }
    // get first component in the stack of a given type
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
    // get all components of an interface type
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
    // remove first component of a given type from the stack and return it
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
    // push an asset onyo the stack
    template <typename T, typename... Args>
    void push_asset(Args... args)
    {
        Referenced<Asset> asset = std::dynamic_pointer_cast<Asset>(Renderer::API->GenRef<T>(args...));
        if (asset)
        {
            assets_.push_back(asset);
        }
        // nothing pushed if T is not an asset
    }
    // get all assets of a type
    template <typename T>
    std::vector<T *> get_assets()
    {
        std::vector<T *> assets{};
        for (auto asset_it = asset_begin(); asset_it != asset_end(); asset_it++)
        {
            if (auto asset = std::dynamic_pointer_cast<T>(*asset_it))
            {
                assets.push_back(asset.get());
            }
        }
        return assets;
    }
        // get first assets of a type in the stack
    template <typename T>
    T * get_asset()
    {
        for (auto asset_it = asset_begin(); asset_it != asset_end(); asset_it++)
        {
            if (auto asset = std::dynamic_pointer_cast<T>(*asset_it))
            {
                return asset.get();
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

    std::vector<Referenced<Asset>>::const_iterator asset_begin() const { return assets_.begin(); }
    std::vector<Referenced<Asset>>::const_iterator asset_end() const { return assets_.end(); }
    std::vector<Referenced<Asset>>::const_reverse_iterator asset_rbegin() const { return assets_.rbegin(); }
    std::vector<Referenced<Asset>>::const_reverse_iterator asset_rend() const { return assets_.rend(); }
    std::vector<Referenced<Asset>>::iterator asset_begin() { return assets_.begin(); }
    std::vector<Referenced<Asset>>::iterator asset_end() { return assets_.end(); }
    std::vector<Referenced<Asset>>::reverse_iterator asset_rbegin() { return assets_.rbegin(); }
    std::vector<Referenced<Asset>>::reverse_iterator asset_rend() { return assets_.rend(); }

private:
    std::vector<Referenced<Component>> components_;
    std::vector<Referenced<Asset>> assets_;
};
} // namespace mare

#endif