#ifndef LAYER
#define LAYER

// MARE
#include "mare/Renderer.hpp"
#include "mare/Camera.hpp"

namespace mare
{
class Layer
{
public:
    Layer() : m_camera(nullptr) {}
    virtual ~Layer()
    {
        if (m_camera)
        {
            delete m_camera;
            m_camera = nullptr;
        }
    }

    virtual bool render(double current_time, double delta_time) { return true; }

    virtual bool on_key(const RendererInput &input) { return false; }
    virtual bool on_mouse_button(const RendererInput &input) { return false; }
    virtual bool on_mouse_move(const RendererInput &input) { return false; }
    virtual bool on_mouse_wheel(const RendererInput &input) { return false; }
    virtual bool on_resize(const RendererInput &input) { return false; }

    inline void set_camera(Camera *camera) { m_camera = camera; }
    inline Camera *get_camera() { return m_camera; }

private:
    Camera *m_camera;
};
} // namespace mare

#endif