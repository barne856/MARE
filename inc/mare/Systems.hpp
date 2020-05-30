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
/**
 * @brief An abstract interface to a PhysicsSystem.
 * @details A PhysicsSystem provides a callback update(float, Entity*) that will
 * be called once every frame if the System is active. A System is active if it
 * is attached to an Entity that is attached to the active Scene.
 * @see Entity
 * @see Scene
 * @see System
 */
class IPhysicsSystem : public System {
public:
  /**
   * @brief Virtual destructor for the IPhysicsSystem object
   */
  virtual ~IPhysicsSystem() {}
  /**
   * @brief The update callback function to be implemented by the derived
   * System.
   *
   * @param dt The amount of time in seconds passed since the last frame.
   * @param entity A pointer to the Entity this System is attached to.
   */
  virtual void update(float dt, Entity *entity) = 0;
};

/**
 * @brief A template base class used to implement a PhysicsSystem which operates
 * on one or more specific components or entities.
 *
 * @tparam <Ts> The types of Components or Entities that the PhysicsSystem will
 * operate on.
 */
template <class... Ts> class PhysicsSystem : public IPhysicsSystem {
public:
  /**
   * @brief The overriden function from IPhysicsSystem that will forward its
   * callbacks to the templated update method.
   *
   * @param dt The amount of time in seconds since the previous frame.
   * @param entity A pointer to the Entity this System is attached to.
   */
  virtual void update(float dt, Entity *entity) final {
    update(dt, dynamic_cast<Ts *>(entity)...);
  }
  /**
   * @brief An abstract virtual function implemented by the PhysicsSystem.
   *
   * @param dt The amount of time in seconds since the previous frame.
   * @param derived_entities A pointer to the Components or Entites this System
   * operates on and are base classes of the Entity this System is attached
   * to.
   */
  virtual void update(float dt, Ts *... derived_entities) = 0;
};

/**
 * @brief An abstract interface to a RenderSystem.
 * @details A RenderSystem provides a callback render(float, Camera*, Entity*)
 * that will be called once every frame if the System is active. A System is
 * active if it is attached to an Entity that is attached to the active Scene.
 * @see Entity
 * @see Scene
 * @see System
 */
class IRenderSystem : public System {
public:
  /**
   * @brief Virtual destructor of the IRenderSystem object
   */
  virtual ~IRenderSystem() {}
  /**
   * @brief The render callback function to be implemented by the derived
   * System.
   *
   * @param dt The amount of time in seconds since the last frame.
   * @param camera A pointer to the Camera used to render.
   * @param entity A pointer to the Entity with this System attached.
   */
  virtual void render(float dt, Camera *camera, Entity *entity) = 0;
};
/**
 * @brief A template base class used to implement a RenderSystem which operates
 * on one or more specific Components or Entities.
 *
 * @tparam <Ts> The types of Components or Entities that the RenderSystem will
 * operate on.
 */
template <class... Ts> class RenderSystem : public IRenderSystem {
public:
  /**
   * @brief The overriden function from IRenderSystem that will forward its
   * callbacks to the templated render method.
   *
   * @param dt The amount of time in seconds since the previous frame.
   * @param camera A pointer to the Camera to render with.
   * @param entity A pointer to the Entity this System is attached to.
   */
  virtual void render(float dt, Camera *camera, Entity *entity) final {
    render(dt, camera, dynamic_cast<Ts *>(entity)...);
  }
  /**
   * @brief An abstract virtual function implemented by the RenderSystem.
   *
   * @param dt The amount of time in seconds since the previous frame.
   * @param camera A pointer to the Camera to render with.
   * @param derived_entities A pointer to the Components or Entites this System
   * operates on and are base classes of the Entity this System is attached
   * to.
   */
  virtual void render(float dt, Camera *camera, Ts *... derived_entities) = 0;
};

/**
 * @brief An abstract interface to a ControlsSystem.
 * @details A ControlsSystem provides callbacks:
 *  - on_key(const RendererInput&, Entity*)
 *    - This callback is executed whenever a key on the keyboard is pressed.
 *  - on_mouse_button(const RendererInput&, Entity*)
 *    - This callback is executed whenever a mouse button is pressed.
 *  - on_mouse_move(const RendererInput&, Entity*)
 *    - This callback is executed whenever the mouse moves.
 *  - on_mouse_wheel(const RendererInput&, Entity*)
 *    - This callback is executed whenever the mouse wheel moves.
 *  - on_resize(const RendererInput&, Entity*)
 *    - This callback is executed whenever the window is resized.
 *  - on_char(unsigned char character, Entity*)
 *    - This callback is executed whenever the operating system's text input
 *      system produces a character.
 *
 * RendererInput is provided by the implemented Rendering API. This type of
 * System will have its callbacks executed whenever there is user input if the
 * System is active. A System is active if it is attached to an Entity that is
 * attached to the active Scene.
 * @see Entity
 * @see Scene
 * @see System
 * @see RendererInput
 */
class IControlsSystem : public System {
public:
  /**
   * @brief Virtual destructor of the IControlsSystem object
   */
  virtual ~IControlsSystem() {}
  /**
   * @brief A callback function to be implemented by the derived
   * System.
   *
   * @param input A const reference the the RendererInput provided by the
   * implemented Rendering API.
   * @param entity A pointer to the Entity with this System attached.
   * @return false will propogate the event down the active Scene's Layer stack
   * and those Layer's System stacks and finally the Scene's System stack until
   * a System returns true or the end of the stacks are reached.
   * @return true will signal the event is handeled and stop the propogation of
   * the event.
   * @see IControlsSystem
   */
  virtual bool on_key(const RendererInput &input, Entity *entity) {
    return false;
  }
  /**
   * @brief A callback function to be implemented by the derived
   * System.
   *
   * @param input A const reference the the RendererInput provided by the
   * implemented Rendering API.
   * @param entity A pointer to the Entity with this System attached.
   * @return false will propogate the event down the active Scene's Layer stack
   * and those Layer's System stacks and finally the Scene's System stack until
   * a System returns true or the end of the stacks are reached.
   * @return true will signal the event is handeled and stop the propogation of
   * the event.
   * @see IControlsSystem
   */
  virtual bool on_mouse_button(const RendererInput &input, Entity *entity) {
    return false;
  }
  /**
   * @brief A callback function to be implemented by the derived
   * System.
   *
   * @param input A const reference the the RendererInput provided by the
   * implemented Rendering API.
   * @param entity A pointer to the Entity with this System attached.
   * @return false will propogate the event down the active Scene's Layer stack
   * and those Layer's System stacks and finally the Scene's System stack until
   * a System returns true or the end of the stacks are reached.
   * @return true will signal the event is handeled and stop the propogation of
   * the event.
   * @see IControlsSystem
   */
  virtual bool on_mouse_move(const RendererInput &input, Entity *entity) {
    return false;
  }
  /**
   * @brief A callback function to be implemented by the derived
   * System.
   *
   * @param input A const reference the the RendererInput provided by the
   * implemented Rendering API.
   * @param entity A pointer to the Entity with this System attached.
   * @return false will propogate the event down the active Scene's Layer stack
   * and those Layer's System stacks and finally the Scene's System stack until
   * a System returns true or the end of the stacks are reached.
   * @return true will signal the event is handeled and stop the propogation of
   * the event.
   * @see IControlsSystem
   */
  virtual bool on_mouse_wheel(const RendererInput &input, Entity *entity) {
    return false;
  }
  /**
   * @brief A callback function to be implemented by the derived
   * System.
   *
   * @param input A const reference the the RendererInput provided by the
   * implemented Rendering API.
   * @param entity A pointer to the Entity with this System attached.
   * @return false will propogate the event down the active Scene's Layer stack
   * and those Layer's System stacks and finally the Scene's System stack until
   * a System returns true or the end of the stacks are reached.
   * @return true will signal the event is handeled and stop the propogation of
   * the event.
   * @see IControlsSystem
   */
  virtual bool on_resize(const RendererInput &input, Entity *entity) {
    return false;
  }
  /**
   * @brief A callback function to be implemented by the derived System.
   *
   * @param character The character produced by the operating system's text
   * input system.
   * @param entity A pointer to the Entity with this System attached.
   * @return false will propogate the event down the active Scene's Layer stack
   * and those Layer's System stacks and finally the Scene's System stack until
   * a System returns true or the end of the stacks are reached.
   * @return true will signal the event is handeled and stop the propogation of
   * the event.
   * @see IControlsSystem
   */
  virtual bool on_char(char character, Entity *entity) {
    return false;
  }
};
/**
 * @brief A template base class used to implement a ControlsSystem which
 * operates on one or more specific Components or Entities.
 *
 * @tparam <Ts> The types of Components or Entities that the ControlsSystem will
 * operate on.
 */
template <class... Ts> class ControlsSystem : public IControlsSystem {
public:
  /**
   * @brief A virtual function implemented by the ControlsSystem to respond to
   * events by operating on some number of Components and/or Entities.
   *
   * @param input A const reference the the RendererInput provided by the
   * implemented Rendering API.
   * @param derived_entities The Entities or Components to operate on.
   * @return true signals event should propogate.
   * @return false signals event is handled.
   * @see IControlsSystem
   */
  virtual bool on_key(const RendererInput &input, Ts *... derived_entities) {
    return false;
  }
  /**
   * @brief A virtual function implemented by the ControlsSystem to respond to
   * events by operating on some number of Components and/or Entities.
   *
   * @param input A const reference the the RendererInput provided by the
   * implemented Rendering API.
   * @param derived_entities The Entities or Components to operate on.
   * @return true signals event should propogate.
   * @return false signals event is handled.
   * @see IControlsSystem
   */
  virtual bool on_mouse_button(const RendererInput &input,
                               Ts *... derived_entities) {
    return false;
  }
  /**
   * @brief A virtual function implemented by the ControlsSystem to respond to
   * events by operating on some number of Components and/or Entities.
   *
   * @param input A const reference the the RendererInput provided by the
   * implemented Rendering API.
   * @param derived_entities The Entities or Components to operate on.
   * @return true signals event should propogate.
   * @return false signals event is handled.
   * @see IControlsSystem
   */
  virtual bool on_mouse_move(const RendererInput &input,
                             Ts *... derived_entities) {
    return false;
  }
  /**
   * @brief A virtual function implemented by the ControlsSystem to respond to
   * events by operating on some number of Components and/or Entities.
   *
   * @param input A const reference the the RendererInput provided by the
   * implemented Rendering API.
   * @param derived_entities The Entities or Components to operate on.
   * @return true signals event should propogate.
   * @return false signals event is handled.
   * @see IControlsSystem
   */
  virtual bool on_mouse_wheel(const RendererInput &input,
                              Ts *... derived_entities) {
    return false;
  }
  /**
   * @brief A virtual function implemented by the ControlsSystem to respond to
   * events by operating on some number of Components and/or Entities.
   *
   * @param input A const reference the the RendererInput provided by the
   * implemented Rendering API.
   * @param derived_entities The Entities or Components to operate on.
   * @return true signals event should propogate.
   * @return false signals event is handled.
   * @see IControlsSystem
   */
  virtual bool on_resize(const RendererInput &input, Ts *... derived_entities) {
    return false;
  }
  /**
   * @brief A virtual function implemented by the ControlsSystem to respond to
   * events by operating on some number of Components and/or Entities.
   *
   * @param character
   * @param derived_entities The Entities or Components to operate on.
   * @return true signals event should propogate.
   * @return false signals event is handled.
   * @see IControlsSystem
   */
  virtual bool on_char(char character, Ts *... derived_entities) {
    return false;
  }
  /**
   * @brief The overriden function from IRenderSystem that will forward its
   * callbacks to the templated render method.
   *
   * @param input A const reference to the RendererInput provided by the
   * implemented Rendering API.
   * @param entity A pointer to the Entity this System is attached to. Must
   * inherit from <Ts>.
   * @return true signals event should propogate.
   * @return false signals event is handled.
   * @see IControlsSystem
   */
  virtual bool on_key(const RendererInput &input, Entity *entity) final {
    return on_key(input, dynamic_cast<Ts *>(entity)...);
  }
  /**
   * @brief The overriden function from IRenderSystem that will forward its
   * callbacks to the templated render method.
   *
   * @param input A const reference to the RendererInput provided by the
   * implemented Rendering API.
   * @param entity A pointer to the Entity this System is attached to. Must
   * inherit from <Ts>.
   * @return true signals event should propogate.
   * @return false signals event is handled.
   * @see IControlsSystem
   */
  virtual bool on_mouse_button(const RendererInput &input,
                               Entity *entity) final {
    return on_mouse_button(input, dynamic_cast<Ts *>(entity)...);
  }
  /**
   * @brief The overriden function from IRenderSystem that will forward its
   * callbacks to the templated render method.
   *
   * @param input A const reference to the RendererInput provided by the
   * implemented Rendering API.
   * @param entity A pointer to the Entity this System is attached to. Must
   * inherit from <Ts>.
   * @return true signals event should propogate.
   * @return false signals event is handled.
   * @see IControlsSystem
   */
  virtual bool on_mouse_move(const RendererInput &input, Entity *entity) final {
    return on_mouse_move(input, dynamic_cast<Ts *>(entity)...);
  }
  /**
   * @brief The overriden function from IRenderSystem that will forward its
   * callbacks to the templated render method.
   *
   * @param input A const reference to the RendererInput provided by the
   * implemented Rendering API.
   * @param entity A pointer to the Entity this System is attached to. Must
   * inherit from <Ts>.
   * @return true signals event should propogate.
   * @return false signals event is handled.
   * @see IControlsSystem
   */
  virtual bool on_mouse_wheel(const RendererInput &input,
                              Entity *entity) final {
    return on_mouse_wheel(input, dynamic_cast<Ts *>(entity)...);
  }
  /**
   * @brief The overriden function from IRenderSystem that will forward its
   * callbacks to the templated render method.
   *
   * @param input A const reference to the RendererInput provided by the
   * implemented Rendering API.
   * @param entity A pointer to the Entity this System is attached to. Must
   * inherit from <Ts>.
   * @return true signals event should propogate.
   * @return false signals event is handled.
   * @see IControlsSystem
   */
  virtual bool on_resize(const RendererInput &input, Entity *entity) final {
    return on_resize(input, dynamic_cast<Ts *>(entity)...);
  }
  virtual bool on_char(char character, Entity *entity) final {
    return on_char(character, dynamic_cast<Ts *>(entity)...);
  }
};

} // namespace mare

#endif