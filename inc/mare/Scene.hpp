#ifndef SCENE
#define SCENE

// MARE
#include "mare/Renderer.hpp"

namespace mare
{
class Scene
{
public:
    Scene() : m_UI_scene(nullptr) {}
    virtual ~Scene() {}
    virtual void startup() {}
    virtual bool render(double current_time, double delta_time) = 0;
    virtual void shutdown() {}

    virtual void on_key(const RendererInput &input) {}
    virtual void on_mouse_button(const RendererInput &input) {}
    virtual void on_mouse_move(const RendererInput &input) {}
    virtual void on_mouse_wheel(const RendererInput &input) {}

    inline Scene *get_UI() const { return m_UI_scene; }

private:
    Scene *m_UI_scene;
};
} // namespace mare

#endif