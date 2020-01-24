#ifndef LAYER
#define LAYER

// MARE
#include "mare/Renderer.hpp"

namespace mare
{
class Layer
{
public:
    Layer() {}
    virtual ~Layer() {}

    virtual bool render(double current_time, double delta_time) = 0;

    virtual bool on_key(const RendererInput &input) { return false; }
    virtual bool on_mouse_button(const RendererInput &input) { return false; }
    virtual bool on_mouse_move(const RendererInput &input) { return false; }
    virtual bool on_mouse_wheel(const RendererInput &input) { return false; }
};
} // namespace mare

#endif