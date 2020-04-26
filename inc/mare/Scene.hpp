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
    Scene(ProjectionType type);
    virtual ~Scene();

    void push_entity(Referenced<Entity> entity);
    void pull_entity(Referenced<Entity> &entity);
    Referenced<Entity> pull_entity(size_t index);
    Entity* get_entity(size_t index);
    void push_overlay(Referenced<Overlay> overlay);
    void pop_overlay();
    Overlay* get_overlay(size_t index);

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