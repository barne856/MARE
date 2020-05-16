#ifndef SYSTEMS
#define SYSTEMS

// MARE
#include "mare/Mare.hpp"
#include "mare/Renderer.hpp"

// Standard Library
#include <type_traits>

// External Libraries
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

namespace mare {
// forward declaration
class Camera;
class Entity;
class IPhysicsSystem : public System {
public:
  virtual ~IPhysicsSystem() {}
  virtual void update(float dt, Entity *entity) = 0;
};
template <class... Ts> class PhysicsSystem : public IPhysicsSystem {

public:
  virtual void update(float dt, Entity *entity) final {
    update(dt, dynamic_cast<Ts *>(entity)...);
  }
  virtual void update(float dt, Ts *... derived_entities) = 0;
};

class IRenderSystem : public System {
public:
  virtual ~IRenderSystem() {}
  virtual void render(float dt, Camera *camera, Entity *entity) = 0;
};
template <class... Ts> class RenderSystem : public IRenderSystem {

public:
  virtual void render(float dt, Camera *camera, Entity *entity) final {
    render(dt, camera, dynamic_cast<Ts *>(entity)...);
  }
  virtual void render(float dt, Camera *camera, Ts *... derived_entities) = 0;
};

class IControlsSystem : public System {
public:
  virtual ~IControlsSystem() {}
  virtual bool on_key(const RendererInput &input, Entity *entity) {
    return false;
  }
  virtual bool on_mouse_button(const RendererInput &input, Entity *entity) {
    return false;
  }
  virtual bool on_mouse_move(const RendererInput &input, Entity *entity) {
    return false;
  }
  virtual bool on_mouse_wheel(const RendererInput &input, Entity *entity) {
    return false;
  }
  virtual bool on_resize(const RendererInput &input, Entity *entity) {
    return false;
  }
};
template <class... Ts> class ControlsSystem : public IControlsSystem {

public:
  virtual bool on_key(const RendererInput &input, Ts *... derived_entities) {
    return false;
  }
  virtual bool on_mouse_button(const RendererInput &input,
                               Ts *... derived_entities) {
    return false;
  }
  virtual bool on_mouse_move(const RendererInput &input,
                             Ts *... derived_entities) {
    return false;
  }
  virtual bool on_mouse_wheel(const RendererInput &input,
                              Ts *... derived_entities) {
    return false;
  }
  virtual bool on_resize(const RendererInput &input, Ts *... derived_entities) {
    return false;
  }
  virtual bool on_key(const RendererInput &input, Entity *entity) final {
    return on_key(input, dynamic_cast<Ts *>(entity)...);
  }
  virtual bool on_mouse_button(const RendererInput &input,
                               Entity *entity) final {
    return on_mouse_button(input, dynamic_cast<Ts*>(entity)...);
  }
  virtual bool on_mouse_move(const RendererInput &input, Entity *entity) final {
    return on_mouse_move(input, dynamic_cast<Ts *>(entity)...);
  }
  virtual bool on_mouse_wheel(const RendererInput &input,
                              Entity *entity) final {
    return on_mouse_wheel(input, dynamic_cast<Ts *>(entity)...);
  }
  virtual bool on_resize(const RendererInput &input, Entity *entity) final {
    return on_resize(input, dynamic_cast<Ts *>(entity)...);
  }
};

} // namespace mare

#endif