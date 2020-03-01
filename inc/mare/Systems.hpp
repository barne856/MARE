#ifndef SYSTEMS
#define SYSTEMS

// MARE
#include "mare/MareUtility.hpp"
#include "mare/Entity.hpp"
#include "mare/Layer.hpp"
#include "mare/Renderer.hpp"

// Standard Library
#include <type_traits>

namespace mare
{

class IPhysicsSystem : public Component
{
public:
    virtual ~IPhysicsSystem() {}
    virtual void update(Entity *entity, float dt) = 0;
};
template <class T>
class PhysicsSystem : public IPhysicsSystem
{
    static_assert(std::is_base_of<Entity, T>::value, "Type must be derived from Entity.");

public:
    virtual void update(Entity *entity, float dt) final
    {
        update(static_cast<T *>(entity), dt);
    }
    virtual void update(T *derived_entity, float dt) = 0;
};

class IRenderSystem : public Component
{
public:
    virtual ~IRenderSystem() {}
    virtual void render(Entity *entity, const Layer *layer, float dt) = 0;
};
template <class T>
class RenderSystem : public IRenderSystem
{
    static_assert(std::is_base_of<Entity, T>::value, "Type must be derived from Entity.");

public:
    virtual void render(Entity *entity, const Layer *layer, float dt) final
    {
        render(static_cast<T *>(entity), layer, dt);
    }
    virtual void render(T *derived_entity, const Layer *layer, float dt) = 0;
};

class IControlsSystem : public Component
{
public:
    virtual ~IControlsSystem() {}
    virtual bool on_key(Entity *entity, const RendererInput &input) {return false;}
    virtual bool on_mouse_button(Entity *entity, const RendererInput &input)  {return false;}
    virtual bool on_mouse_move(Entity *entity, const RendererInput &input)  {return false;}
    virtual bool on_mouse_wheel(Entity *entity, const RendererInput &input)  {return false;}
    virtual bool on_resize(Entity *entity, const RendererInput &input)  {return false;}
};
template <class T>
class ControlsSystem : public IControlsSystem
{
    static_assert(std::is_base_of<Entity, T>::value, "Type must be derived from Entity.");
public:
    virtual bool on_key(T *derived_entity, const RendererInput &input)  {return false;}
    virtual bool on_mouse_button(T *derived_entity, const RendererInput &input)  {return false;}
    virtual bool on_mouse_move(T *derived_entity, const RendererInput &input)  {return false;}
    virtual bool on_mouse_wheel(T *derived_entity, const RendererInput &input)  {return false;}
    virtual bool on_resize(T *derived_entity, const RendererInput &input)  {return false;}
    virtual bool on_key(Entity *entity, const RendererInput &input) final
    {
        return on_key(static_cast<T *>(entity), input);
    }
    virtual bool on_mouse_button(Entity *entity, const RendererInput &input) final
    {
        return on_mouse_button(static_cast<T *>(entity), input);
    }
    virtual bool on_mouse_move(Entity *entity, const RendererInput &input) final
    {
        return on_mouse_move(static_cast<T *>(entity), input);
    }
    virtual bool on_mouse_wheel(Entity *entity, const RendererInput &input) final
    {
        return on_mouse_wheel(static_cast<T *>(entity), input);
    }
    virtual bool on_resize(Entity *entity, const RendererInput &input) final
    {
        return on_resize(static_cast<T *>(entity), input);
    }
};
} // namespace mare

#endif