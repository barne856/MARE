#ifndef SCENE
#define SCENE

// MARE
#include "mare/Mare.hpp"
#include "mare/Layer.hpp"
#include "mare/Overlay.hpp"

// Standard Library
#include <vector>

namespace mare
{
    class Scene : public Layer
    {
    public:
        Scene(ProjectionType type) : Layer(type) {}
        virtual ~Scene() {}

        template <typename T, typename... Args>
        void push_entity(Args... args)
        {
            Referenced<Entity> entity = std::dynamic_pointer_cast<Entity>(Renderer::API->GenRef<T>(args...));
            if (entity)
            {
                entity_stack_.push_back(entity);
            }
            // nothing pushed if T is not an entity
        }
        template <typename T>
        Referenced<T> pull_entity()
        {
            for (auto entity_it = entity_begin(); entity_it != entity_end(); entity_it++)
            {
                if (auto entity = std::dynamic_pointer_cast<T>(*entity_it))
                {
                    Referenced<T> pulled_entity = std::move(entity);
                    entity_stack_.erase(entity_it);
                    return pulled_entity;
                }
            }
            return nullptr;
        }
        void pop_entity()
        {
            entity_stack_.pop_back();
        }
        template <typename T>
        T *get_entity()
        {
            for (auto entity_it = entity_begin(); entity_it != entity_end(); entity_it++)
            {
                if (auto entity = std::dynamic_pointer_cast<T>(*entity_it))
                {
                    return entity.get();
                }
            }
            return nullptr;
        }
        template <typename T>
        std::vector<T *> get_entities()
        {
            std::vector<T *> entities{};
            for (auto entity_it = entity_begin(); entity_it != entity_end(); entity_it++)
            {
                if (auto entity = std::dynamic_pointer_cast<T>(*entity_it))
                {
                    entities.push_back(entity.get());
                }
            }
            return entities;
        }

        template <typename T, typename... Args>
        void push_overlay(Args... args)
        {
            Referenced<Overlay> overlay = std::dynamic_pointer_cast<Overlay>(Renderer::API->GenRef<T>(args...));
            if (overlay)
            {
                overlay_stack_.push_back(overlay);
            }
            // nothing pushed if T is not an overlay
        }
        template <typename T>
        Referenced<T> pull_overlay()
        {
            for (auto overlay_it = overlay_begin(); overlay_it != overlay_end(); overlay_it++)
            {
                if (auto overlay = std::dynamic_pointer_cast<T>(*overlay_it))
                {
                    Referenced<T> pulled_overlay = std::move(overlay);
                    entity_stack_.erase(overlay_it);
                    return pulled_overlay;
                }
            }
            return nullptr;
        }
        void pop_overlay()
        {
            overlay_stack_.pop_back();
        }
        template <typename T>
        T *get_overlay()
        {
            for (auto overlay_it = overlay_begin(); overlay_it != overlay_end(); overlay_it++)
            {
                if (auto overlay = std::dynamic_pointer_cast<T>(*overlay_it))
                {
                    return overlay.get();
                }
            }
            return nullptr;
        }
        template <typename T>
        std::vector<T *> get_overlays()
        {
            std::vector<T *> overlays{};
            for (auto overlay_it = overlay_begin(); overlay_it != overlay_end(); overlay_it++)
            {
                if (auto overlay = std::dynamic_pointer_cast<T>(*overlay_it))
                {
                    overlays.push_back(overlay.get());
                }
            }
            return overlays;
        }

        std::vector<Referenced<Entity>>::const_iterator entity_begin() const { return entity_stack_.begin(); }
        std::vector<Referenced<Entity>>::const_iterator entity_end() const { return entity_stack_.end(); }
        std::vector<Referenced<Entity>>::const_reverse_iterator entity_rbegin() const { return entity_stack_.rbegin(); }
        std::vector<Referenced<Entity>>::const_reverse_iterator entity_rend() const { return entity_stack_.rend(); }
        std::vector<Referenced<Entity>>::iterator entity_begin() { return entity_stack_.begin(); }
        std::vector<Referenced<Entity>>::iterator entity_end() { return entity_stack_.end(); }
        std::vector<Referenced<Entity>>::reverse_iterator entity_rbegin() { return entity_stack_.rbegin(); }
        std::vector<Referenced<Entity>>::reverse_iterator entity_rend() { return entity_stack_.rend(); }

        std::vector<Referenced<Overlay>>::const_iterator overlay_begin() const { return overlay_stack_.begin(); }
        std::vector<Referenced<Overlay>>::const_iterator overlay_end() const { return overlay_stack_.end(); }
        std::vector<Referenced<Overlay>>::const_reverse_iterator overlay_rbegin() const { return overlay_stack_.rbegin(); }
        std::vector<Referenced<Overlay>>::const_reverse_iterator overlay_rend() const { return overlay_stack_.rend(); }
        std::vector<Referenced<Overlay>>::iterator overlay_begin() { return overlay_stack_.begin(); }
        std::vector<Referenced<Overlay>>::iterator overlay_end() { return overlay_stack_.end(); }
        std::vector<Referenced<Overlay>>::reverse_iterator overlay_rbegin() { return overlay_stack_.rbegin(); }
        std::vector<Referenced<Overlay>>::reverse_iterator overlay_rend() { return overlay_stack_.rend(); }

    private:
        std::vector<Referenced<Entity>> entity_stack_{};
        std::vector<Referenced<Overlay>> overlay_stack_{};
    };
} // namespace mare

#endif