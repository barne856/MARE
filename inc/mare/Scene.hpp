#ifndef SCENE
#define SCENE

// MARE
#include "mare/Layer.hpp"
#include "mare/Overlay.hpp"
#include "mare/Object.hpp"

// Standard Library
#include <vector>

namespace mare
{
class Scene : public Layer
{
public:
    Scene();
    virtual ~Scene();

    virtual bool render(double current_time, double delta_time) override { return true; }
    virtual bool on_key(const RendererInput &input) { return false; }
    virtual bool on_mouse_button(const RendererInput &input) { return false; }
    virtual bool on_mouse_move(const RendererInput &input) { return false; }
    virtual bool on_mouse_wheel(const RendererInput &input) { return false; }
    virtual bool on_resize(const RendererInput &input) { return false; }

    shader_data_type get_widget_value(size_t overlay_index, size_t widget_index);
    Layer *get_layer(size_t index);
    std::vector<Layer *> *get_layer_stack();
    void push_layer(Layer *layer);

private:
    std::vector<Layer *> *m_layer_stack;
};
} // namespace mare

#endif