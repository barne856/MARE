#ifndef WIDGET
#define WIDGET

// MARE
#include "mare/Object.hpp"
#include "mare/Layer.hpp"
#include "mare/MareUtility.hpp"

// External Libraries
#include "glm.hpp"

// Standar Library
#include <variant>

namespace mare
{

struct Rect
{
    float &left() { return m_values[0]; }
    float &bottom() { return m_values[1]; }
    float &right() { return m_values[2]; }
    float &top() { return m_values[3]; }

    void set_rect(glm::vec2 bottom_left, glm::vec2 top_right)
    {
        m_values = glm::vec4(bottom_left, top_right);
    }

    float operator[](unsigned i) const { return m_values[i]; }
    float &operator[](unsigned i) { return m_values[i]; }

private:
    glm::vec4 m_values{};
};

class Widget : public Layer, public Object
{
public:
    using Object::render;
    Widget(Layer* layer);
    virtual ~Widget();

    virtual bool render(double current_time, double delta_time) final { return true; }
    virtual bool on_key(const RendererInput &input) override { return false; }
    virtual bool on_mouse_button(const RendererInput &input) override { return false; }
    virtual bool on_mouse_move(const RendererInput &input) override { return false; }
    virtual bool on_mouse_wheel(const RendererInput &input) override { return false; }
    virtual bool on_resize(const RendererInput &input) final { return false; }

    glm::vec2 get_widget_coords();
    bool is_in_bounds();
    shader_data_type get_value();
    virtual void set_value(shader_data_type value);

protected:
    shader_data_type value;
    Rect bounds;
    Layer* base_layer;
};
} // namespace mare

#endif