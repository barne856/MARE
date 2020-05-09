#ifndef SCENE
#define SCENE

// MARE
#include "mare/Layer.hpp"
#include "mare/Mare.hpp"
#include "mare/Overlay.hpp"


// Standard Library
#include <vector>

namespace mare {
class Scene : public Layer {
public:
  Scene(ProjectionType type) : Layer(type) {}
  virtual ~Scene() {}

  // Entity Template Functions--------------------------------

  // generate single entity and push it onto the stack
  template <typename T, typename... Args> void gen_entity(Args... args) {
    // generate entity reference of type T and cast to base type
    Referenced<Entity> ent =
        std::dynamic_pointer_cast<Entity>(gen_ref<T>(args...));
    // if T is an entity push the entity onto the stack
    if (ent)
      entities_.push_back(ent);
  }
  // generate multiple entities and push them onto the stack
  template <typename T, typename... Args>
  void gen_entities(uint32_t count, Args... args) {
    for (uint32_t i = 0; i < count; i++) {
      // generate entity reference of type T and cast to base type
      Referenced<Entity> ent =
          std::dynamic_pointer_cast<Entity>(gen_ref<T>(args...));
      // if T is an entity push the entity onto the stack
      if (ent)
        entities_.push_back(ent);
    }
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
    std::vector<T *> entities{};
    for (auto ent_it = entity_begin(); ent_it != entity_end(); ent_it++) {
      if (auto entity = std::dynamic_pointer_cast<T>(*ent_it)) {
        Referenced<T> pulled_entity = std::move(entity);
        entities_.erase(ent_it);
        entities.push_back(pulled_entity);
      }
    }
    return entities;
  }

  // Overlay Template Functions--------------------------------

  // generate single overlay and push it onto the stack
  template <typename T, typename... Args> void gen_overlay(Args... args) {
    // generate overlay reference of type T and cast to base type
    Referenced<Overlay> over =
        std::dynamic_pointer_cast<Overlay>(gen_ref<T>(args...));
    // if T is a overlay push the asset onto the stack
    if (over)
      overlays_.push_back(over);
  }
  // generate multiple overlays and push them onto the stack
  template <typename T, typename... Args>
  void gen_overlays(uint32_t count, Args... args) {
    for (uint32_t i = 0; i < count; i++) {
      // generate overlay reference of type T and cast to base type
      Referenced<Overlay> over =
          std::dynamic_pointer_cast<Overlay>(gen_ref<T>(args...));
      // if T is a overlay push the asset onto the stack
      if (over)
        overlays_.push_back(over);
    }
  }
  // get first overlay in the stack of a given type
  template <typename T> T *get_overlay() {
    for (auto over_it = overlay_begin(); over_it != overlay_end(); over_it++) {
      if (auto overlay = std::dynamic_pointer_cast<T>(*over_it)) {
        return overlay.get();
      }
    }
    return nullptr;
  }
  // get all overlays of an interface or concrete type
  template <typename T> std::vector<T *> get_overlays() {
    std::vector<T *> overlays{};
    for (auto over_it = overlay_begin(); over_it != overlay_end(); over_it++) {
      if (auto overlay = std::dynamic_pointer_cast<T>(*over_it)) {
        overlays.push_back(overlay.get());
      }
    }
    return overlays;
  }
  // push an existing reference onto the stack
  template <typename T> void push_overlay(Referenced<T> overlay) {
    Referenced<Overlay> over = std::dynamic_pointer_cast<Overlay>(overlay);
    if (over)
      assets_.push_back(over);
  }
  // push a vector of existing references onto the stack
  template <typename T> void push_overlays(std::vector<Referenced<T>> overlays) {
    for (size_t i = 0; i < overlays.size(); i++) {
      Referenced<Overlay> over =
          std::dynamic_pointer_cast<Overlay>(overlays[i]);
      if (over)
        overlays_.push_back(over);
    }
  }
  // remove first overlay of a given type from the stack and return it
  template <typename T> Referenced<T> pull_overlay() {
    for (auto over_it = overlay_begin(); over_it != overlay_end(); over_it++) {
      if (auto overlay = std::dynamic_pointer_cast<T>(*over_it)) {
        Referenced<T> pulled_overlay = std::move(overlay);
        overlays_.erase(over_it);
        return pulled_overlay;
      }
    }
    return nullptr;
  }
  // remove all overlays of a given type from the stack and return them as a
  // vector of references
  template <typename T> std::vector<Referenced<T>> pull_overlays() {
    std::vector<T *> overlays{};
    for (auto over_it = overlay_begin(); over_it != overlay_end(); over_it++) {
      if (auto overlay = std::dynamic_pointer_cast<T>(*over_it)) {
        Referenced<T> pulled_overlay = std::move(overlay);
        overlays_.erase(over_it);
        overlays.push_back(pulled_overlay);
      }
    }
    return overlays;
  }
  //----------------------------

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

  std::vector<Referenced<Overlay>>::const_iterator overlay_begin() const {
    return overlays_.begin();
  }
  std::vector<Referenced<Overlay>>::const_iterator overlay_end() const {
    return overlays_.end();
  }
  std::vector<Referenced<Overlay>>::const_reverse_iterator
  overlay_rbegin() const {
    return overlays_.rbegin();
  }
  std::vector<Referenced<Overlay>>::const_reverse_iterator
  overlay_rend() const {
    return overlays_.rend();
  }
  std::vector<Referenced<Overlay>>::iterator overlay_begin() {
    return overlays_.begin();
  }
  std::vector<Referenced<Overlay>>::iterator overlay_end() {
    return overlays_.end();
  }
  std::vector<Referenced<Overlay>>::reverse_iterator overlay_rbegin() {
    return overlays_.rbegin();
  }
  std::vector<Referenced<Overlay>>::reverse_iterator overlay_rend() {
    return overlays_.rend();
  }

private:
  std::vector<Referenced<Entity>> entities_{};
  std::vector<Referenced<Overlay>> overlays_{};
};
} // namespace mare

#endif