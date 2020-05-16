#ifndef LAYER
#define LAYER

// MARE
#include "mare/Entities/Camera.hpp"

namespace mare {
class Layer : public Camera {
public:
  Layer(ProjectionType type) : Camera(type) {}
  virtual ~Layer() {}
  virtual void on_enter() = 0;
  virtual void render(float dt) = 0;
  virtual void on_exit() = 0;

  // Entity Template Functions--------------------------------

  // generate single entity and push it onto the stack
  template <typename T, typename... Args> Referenced<T> gen_entity(Args... args) {
    // generate entity reference of type T and cast to base type
    Referenced<T> ent = gen_ref<T>(args...);
    if (ent)
    {
      entities_.push_back(ent);
      return ent;
    }
    return nullptr;
  }
  // generate multiple entities and push them onto the stack
  template <typename T, typename... Args>
  std::vector<Referenced<T>> gen_entities(uint32_t count, Args... args) {
    std::vector<Referenced<T>> ents{};
    for (uint32_t i = 0; i < count; i++) {
      // generate entity reference of type T and cast to base type
      Referenced<T> ent = gen_ref<T>(args...);
      if (ent)
      {
        entities_.push_back(ent);
        ents.push_back(ent);
      }
    }
    return ents;
  }
  // get first entity in the stack of a given type
  template <typename T> T *get_entity() {
    for (auto ent_it = entity_begin(); ent_it != entity_end(); ent_it++) {
      if (auto entity = std::dynamic_pointer_cast<T>(*ent_it)) {
        return entity.get();
      }
    }
    return nullptr;
  }
  // get all entities of an interface type
  template <typename T> std::vector<T *> get_entities() {
    std::vector<T *> entities{};
    for (auto ent_it = entity_begin(); ent_it != entity_end(); ent_it++) {
      if (auto entity = std::dynamic_pointer_cast<T>(*ent_it)) {
        entities.push_back(entity.get());
      }
    }
    return entities;
  }
  // push an existing reference onto the stack
  template <typename T> void push_entity(Referenced<T> entity) {
    Referenced<Entity> ent = std::dynamic_pointer_cast<Entity>(entity);
    if (ent)
      entities_.push_back(ent);
  }
  // push a vector of existing references onto the stack
  template <typename T>
  void push_entities(std::vector<Referenced<T>> entities) {
    for (size_t i = 0; i < entities.size(); i++) {
      Referenced<Entity> ent = std::dynamic_pointer_cast<Entity>(entities[i]);
      if (ent)
        entities_.push_back(ent);
    }
  }
  // remove first entity of a given type from the stack and return it
  template <typename T> Referenced<T> pull_entity() {
    for (auto ent_it = entity_begin(); ent_it != entity_end(); ent_it++) {
      if (auto entity = std::dynamic_pointer_cast<T>(*ent_it)) {
        Referenced<T> pulled_entity = std::move(entity);
        entities_.erase(ent_it);
        return pulled_entity;
      }
    }
    return nullptr;
  }
  // remove all entities of a given type from the stack and return them as a
  // vector of references
  template <typename T> std::vector<Referenced<T>> pull_entities() {
    std::vector<Referenced<T>> entities{};
    for (auto ent_it = entity_begin(); ent_it != entity_end(); ent_it++) {
      if (auto entity = std::dynamic_pointer_cast<T>(*ent_it)) {
        Referenced<T> pulled_entity = std::move(entity);
        entities_.erase(ent_it);
        entities.push_back(pulled_entity);
      }
    }
    return entities;
  }

  std::vector<Referenced<Entity>>::const_iterator entity_begin() const {
    return entities_.begin();
  }
  std::vector<Referenced<Entity>>::const_iterator entity_end() const {
    return entities_.end();
  }
  std::vector<Referenced<Entity>>::const_reverse_iterator
  entity_rbegin() const {
    return entities_.rbegin();
  }
  std::vector<Referenced<Entity>>::const_reverse_iterator entity_rend() const {
    return entities_.rend();
  }
  std::vector<Referenced<Entity>>::iterator entity_begin() {
    return entities_.begin();
  }
  std::vector<Referenced<Entity>>::iterator entity_end() {
    return entities_.end();
  }
  std::vector<Referenced<Entity>>::reverse_iterator entity_rbegin() {
    return entities_.rbegin();
  }
  std::vector<Referenced<Entity>>::reverse_iterator entity_rend() {
    return entities_.rend();
  }

private:
  std::vector<Referenced<Entity>> entities_{};
};
} // namespace mare

#endif