#ifndef LAYER
#define LAYER

// MARE
#include "mare/Entities/Camera.hpp"

// Standard Library
#include <algorithm> // std::swap

namespace mare {
/**
 * @brief A Layer object that is the abstract interface for all Scenes and UI
 * Layers.
 * @details The Layer class provides the abstract interface for all user created
 * Scenes and UI Layers. A Layer has an *Entity stack* that contains all of the
 * Entities in a Layer. A Layer is itself an Entity and therefore has its own
 * System stack and can inherit from Components. Generally these Systems and
 * Components will operate on all of the Entities in the Layer and act as
 * *Meta-Components* and *Meta-Systems* but they can also be used to operate on
 * the Camera.
 *
 * All Layers inherit from the Camera Entity and thus require a ProjectionType
 * to be supplied on creation. A Layer can also contain Camera Entities on the
 * Entity stack which can be used to *swap* with the parent Camera using the
 * Layer::swap_camera(Referenced<Camera>) function. This is not done by swaping
 * references but by actually swaping the data in the Cameras.
 * @see Entity
 * @see System
 * @see Transform
 */
class Layer : public Camera {
public:
  /**
   * @brief Construct a new Layer object
   *
   * @param type The ProjectionType of the base Camera object
   */
  Layer(ProjectionType type) : Camera(type) {}
  /**
   * @brief Virtual destructor of the Layer object.
   */
  virtual ~Layer() {}
  /**
   * @brief The interface to a function that is executed when a Layer is
   * initially loaded.
   * @details When a new Scene is loaded, the implementation of this function
   * for that Scene is executed and then the implementations of this function
   * for each of the Layers on that Scene's Layer stack are executed.
   * @see Scene
   */
  virtual void on_enter() = 0;
  /**
   * @brief An optional function that if implemented in a Layer will run at the
   * begining of a new frame before any other RenderSystem.
   * @details This function is garunteed to run before any other System on the
   * Sytem stack and is generally used to set up the rendering properties for
   * the Layer such as enabling depth testing, clearing the framebuffer, setting
   * the blending mode, etc. More complex rendering code should be placed in a
   * RenderSystem attached to the Layer. This can be used to set Renderer
   * properties that will not change or update throughout the Layer's lifetime.
   *
   * @param dt The amount of time in seconds since the last frame.
   * @see IRenderSystem
   */
  virtual void render(float dt) {}
  /**
   * @brief The interface to a function that is executed when a Layer is
   * unloaded.
   * @details When a new Scene is loaded, the implementation of this function in
   * the previously loaded Scene is executed after the implementations of
   * this function for each of the Layers on that Scene's Layer stack are
   * executed in reverse order. This can be used to undo any changes made to the
   * Renderer from the on_enter() function before loading the next Scene.
   * @see Scene
   */
  virtual void on_exit() = 0;
  /**
   * @brief Swaps the properties of the Layer's parent Camera with the
   * properties of a Referenced Camera.
   *
   * @param camera The Referenced Camera to swap with.
   * @see Camera
   */
  void swap_camera(Referenced<Camera> camera) {
    ProjectionType temp_type = get_type();
    float temp_fovy = get_fovy();
    float temp_ortho_scale = get_ortho_scale();
    float temp_aspect = get_aspect();
    float temp_ortho_near = get_near_clip_plane_ortho();
    float temp_ortho_far = get_far_clip_plane_ortho();
    float temp_persp_near = get_near_clip_plane_persp();
    float temp_persp_far = get_far_clip_plane_persp();

    set_type(camera->get_type());
    set_fovy(camera->get_fovy());
    set_ortho_scale(camera->get_ortho_scale());
    set_aspect(camera->get_aspect());
    set_near_clip_plane_ortho(camera->get_near_clip_plane_ortho());
    set_far_clip_plane_ortho(camera->get_far_clip_plane_ortho());
    set_near_clip_plane_persp(camera->get_near_clip_plane_persp());
    set_far_clip_plane_persp(camera->get_far_clip_plane_persp());

    camera->set_type(temp_type);
    camera->set_fovy(temp_fovy);
    camera->set_ortho_scale(temp_ortho_scale);
    camera->set_aspect(temp_aspect);
    camera->set_near_clip_plane_ortho(temp_ortho_near);
    camera->set_far_clip_plane_ortho(temp_ortho_far);
    camera->set_near_clip_plane_persp(temp_persp_near);
    camera->set_far_clip_plane_persp(temp_persp_far);
    swap_transform(camera);
  }

  /**
   * @brief Generate an Entity and push it onto the Entity stack.
   * @details This function will generate a new reference to an Entity of type
   * <T> and push it onto the Layer's Entity stack. All Entities that are
   * attached to Layer's Entity stack are active and their Systems receive
   * callbacks from the Engine. A Referenced Entity is returned that can be used
   * and will be deleted when it goes out of scope.
   *
   * @tparam <T> The type of Entity. <T> must be an Entity.
   * @tparam <Args> The type of the arguments for the Contructor of the Entity.
   * @param args The arguments for the Constructor of the Entity.
   * @return The Referenced Entity.
   * @see Entity
   */
  template <typename T, typename... Args>
  Referenced<T> gen_entity(Args... args) {
    Referenced<T> ent = gen_ref<T>(args...);
    if (ent) {
      entities_.push_back(ent);
      return ent;
    }
    return nullptr;
  }
  /**
   * @brief Generate multiple Entities and push them onto the Entity stack.
   * @details This function will create multiple Entities of the same type and
   * push them all onto the Entity stack. A `std::vector` of Referenced Entities
   * is returned that can be used and will be deleted when it goes out of
   * scope.
   *
   * @tparam <T> The type of Entities to generate. <T> must be a Entity.
   * @tparam <Args> The type of the arguments for the Contructor of the Entity.
   * @param args The arguments for the Constructor of the Entity.
   * @param count The number of Entities to generate.
   * @see Entity
   */
  template <typename T, typename... Args>
  std::vector<Referenced<T>> gen_entities(uint32_t count, Args... args) {
    std::vector<Referenced<T>> ents{};
    for (uint32_t i = 0; i < count; i++) {
      Referenced<T> ent = gen_ref<T>(args...);
      if (ent) {
        entities_.push_back(ent);
        ents.push_back(ent);
      }
    }
    return ents;
  }
  /**
   * @brief Get a pointer to the first Entity of type <T> in the Layer's
   * Entity stack.
   *
   * @tparam <T> The type of Entity to get. <T> must be an Entity.
   * @return A pointer to the first Entity of type <T> in the Entity stack.
   * nullptr is returned if no Entity of type <T> exists on the Entity stack.
   * @see Entity
   */
  template <typename T> T *get_entity() {
    for (auto ent_it = entity_begin(); ent_it != entity_end(); ent_it++) {
      if (auto entity = std::dynamic_pointer_cast<T>(*ent_it)) {
        return entity.get();
      }
    }
    return nullptr;
  }
  /**
   * @brief Get a `std::vector` of pointers to all of the Entities of type <T>
   * in the Layer's Entity stack.
   *
   * @tparam <T> The type of Entity to get. <T> must be an Entity.
   * @return A `std::vector` of pointers to all of the Entities of type <T> in
   * the Entity stack. An empty vector is returned if no Entity of type <T>
   * exists on the Entity stack.
   * @see Entity
   */
  template <typename T> std::vector<T *> get_entities() {
    std::vector<T *> entities{};
    for (auto ent_it = entity_begin(); ent_it != entity_end(); ent_it++) {
      if (auto entity = std::dynamic_pointer_cast<T>(*ent_it)) {
        entities.push_back(entity.get());
      }
    }
    return entities;
  }
  /**
   * @brief Push an existing Referenced Entity onto the Layer's Entity stack.
   *
   * @param entity A Referenced Entity.
   * @see Entity
   */
  template <typename T> void push_entity(Referenced<T> entity) {
    entities_.push_back(entity);
  }
  /**
   * @brief Push a `std::vector` of existing Referenced Entities onto the
   * Layer's Entity stack.
   *
   * @param entities A `std::vector` of existing Referenced Entities.
   * @see Entity
   */
  template <typename T>
  void push_entities(std::vector<Referenced<T>> entities) {
    for (size_t i = 0; i < entities.size(); i++) {
      entities_.push_back(entities[i]);
    }
  }
  /**
   * @brief Remove the first Entity of type <T> from the Entity stack and return
   * the Referenced Entity.
   *
   * @tparam <T> The type of Entity to pull.
   * @return The Referenced Entity that was pulled from the Entity stack.
   * @see Entity
   */
  template <typename T> Referenced<T> pull_entity() {
    for (auto ent_it = entity_begin(); ent_it != entity_end(); ent_it++) {
      if (auto entity = std::dynamic_pointer_cast<T>(*ent_it)) {
        Referenced<T> pulled_entity = entity;
        entities_.erase(ent_it);
        return pulled_entity;
      }
    }
    return nullptr;
  }
  /**
   * @brief Remove all of the Entities of type <T> from the Entity stack and
   * return a `std::vector` of the Referenced Entities.
   *
   * @tparam <T> The type of Entities to pull.
   * @return A `std::vector` of the Referenced Entities that were pulled from
   * the Entity stack.
   * @see Entity
   */
  template <typename T> std::vector<Referenced<T>> pull_entities() {
    std::vector<Referenced<T>> entities{};
    for (auto ent_it = entity_begin(); ent_it != entity_end(); ent_it++) {
      if (auto entity = std::dynamic_pointer_cast<T>(*ent_it)) {
        Referenced<T> pulled_entity = entity;
        entities_.erase(ent_it);
        entities.push_back(pulled_entity);
      }
    }
    return entities;
  }
  /**
   * @brief Get a const iterator pointing to the begining of the Entity stack.
   *
   * @return A const iterator pointing to the begining of the Entity stack.
   * @see Entity
   */
  std::vector<Referenced<Entity>>::const_iterator entity_begin() const {
    return entities_.begin();
  }
  /**
   * @brief Get a const iterator pointing to the end of the Entity stack.
   *
   * @return A const iterator pointing to the end of the Entity stack.
   * @see Entity
   */
  std::vector<Referenced<Entity>>::const_iterator entity_end() const {
    return entities_.end();
  }
  /**
   * @brief Get a const reverse iterator pointing to the end of the Entity
   * stack.
   *
   * @return A const reverse iterator pointing to the end of the Entity stack.
   * @see Entity
   */
  std::vector<Referenced<Entity>>::const_reverse_iterator
  entity_rbegin() const {
    return entities_.rbegin();
  }
  /**
   * @brief Get a const reverse iterator pointing to the begining of the Entity
   * stack.
   *
   * @return A const reverse iterator pointing to the begining of the Entity
   * stack.
   * @see Entity
   */
  std::vector<Referenced<Entity>>::const_reverse_iterator entity_rend() const {
    return entities_.rend();
  }
  /**
   * @brief Get a iterator pointing to the begining of the Entity stack.
   *
   * @return An iterator pointing to the begining of the Entity stack.
   * @see Entity
   */
  std::vector<Referenced<Entity>>::iterator entity_begin() {
    return entities_.begin();
  }
  /**
   * @brief Get a iterator pointing to the end of the Entity stack.
   *
   * @return An iterator pointing to the end of the Entity stack.
   * @see Entity
   */
  std::vector<Referenced<Entity>>::iterator entity_end() {
    return entities_.end();
  }
  /**
   * @brief Get a reverse iterator pointing to the end of the Entity
   * stack.
   *
   * @return A reverse iterator pointing to the end of the Entity stack.
   * @see Entity
   */
  std::vector<Referenced<Entity>>::reverse_iterator entity_rbegin() {
    return entities_.rbegin();
  }
  /**
   * @brief Get a reverse iterator pointing to the begining of the Entity
   * stack.
   *
   * @return A reverse iterator pointing to the begining of the Entity stack.
   * @see Entity
   */
  std::vector<Referenced<Entity>>::reverse_iterator entity_rend() {
    return entities_.rend();
  }

private:
  std::vector<Referenced<Entity>> entities_{}; /**< The Entity stack.*/
};
} // namespace mare

#endif