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
    virtual bool render() = 0;

    virtual void on_key(const RendererInput &input) {}
    virtual void on_mouse_button(const RendererInput &input) {}
    virtual void on_mouse_move(const RendererInput &input) {}
    virtual void on_mouse_wheel(const RendererInput &input) {}

    void set_UI(Scene *UI_scene) { m_UI_scene = UI_scene; }

private:
    Scene *m_UI_scene;
};
} // namespace mare

#endif