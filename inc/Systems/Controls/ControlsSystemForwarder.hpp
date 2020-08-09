#ifndef CONTROLSSYSTEMFORWARDER
#define CONTROLSSYSTEMFORWARDER

#include "Entity.hpp"
#include "Systems.hpp"


namespace mare {
class ControlsSystemForwarder : public IControlsSystem {
public:
  ControlsSystemForwarder(Entity *forward_entity)
      : forward_entity_(forward_entity) {}
  bool on_key(const RendererInput &input, Entity *entity) override {
    auto controls_systems = forward_entity_->get_systems<IControlsSystem>();
    for (auto &system : controls_systems) {
      if (system->on_key(input, forward_entity_)) {
        return true;
      }
    }
    return false;
  }
  bool on_mouse_button(const RendererInput &input, Entity *entity) override {
    auto controls_systems = forward_entity_->get_systems<IControlsSystem>();
    for (auto &system : controls_systems) {
      if (system->on_mouse_button(input, forward_entity_)) {
        return true;
      }
    }
    return false;
  }
  bool on_mouse_move(const RendererInput &input, Entity *entity) override {
    auto controls_systems = forward_entity_->get_systems<IControlsSystem>();
    for (auto &system : controls_systems) {
      if (system->on_mouse_move(input, forward_entity_)) {
        return true;
      }
    }
    return false;
  }
  bool on_mouse_wheel(const RendererInput &input, Entity *entity) override {
    auto controls_systems = forward_entity_->get_systems<IControlsSystem>();
    for (auto &system : controls_systems) {
      if (system->on_mouse_wheel(input, forward_entity_)) {
        return true;
      }
    }
    return false;
  }
  bool on_resize(const RendererInput &input, Entity *entity) override {
    auto controls_systems = forward_entity_->get_systems<IControlsSystem>();
    for (auto &system : controls_systems) {
      if (system->on_resize(input, forward_entity_)) {
        return true;
      }
    }
    return false;
  }
  bool on_char(char character, Entity *entity) override {
    auto controls_systems = forward_entity_->get_systems<IControlsSystem>();
    for (auto &system : controls_systems) {
      if (system->on_char(character, forward_entity_)) {
        return true;
      }
    }
    return false;
  }

private:
  Entity *forward_entity_;
};
} // namespace mare

#endif