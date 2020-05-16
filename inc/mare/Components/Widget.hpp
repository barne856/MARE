#ifndef WIDGET
#define WIDGET

// MARE
#include "mare/Entity.hpp"
#include "mare/Layer.hpp"
#include "mare/Mare.hpp"

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

class Widget : virtual public Transform
{
public:
    Widget(Layer* layer);
    virtual ~Widget();

    glm::vec2 get_widget_coords();
    bool is_in_bounds();
    shader_data_type get_value();
    virtual void set_value(shader_data_type value);
    void set_layer(Layer* layer);
    Layer* get_layer();

protected:
    shader_data_type value;
    Rect bounds;
    Layer* base_layer;
};

Widget::Widget(Layer* layer) : value({}), bounds({}), base_layer(layer) {}
Widget::~Widget() {}
glm::vec2 Widget::get_widget_coords()
{
    if (base_layer)
    {
        return glm::vec2(glm::inverse(transform_) * glm::vec4(Renderer::raycast(base_layer), 1.0f));
    }
    return glm::vec2(0.0f, 0.0f);
}
bool Widget::is_in_bounds()
{
    if (base_layer)
    {
        glm::vec2 v1 = glm::vec2(transform_ * glm::vec4(bounds.left(), bounds.bottom(), 0.0f, 1.0f));
        glm::vec2 v2 = glm::vec2(transform_ * glm::vec4(bounds.right(), bounds.bottom(), 0.0f, 1.0f));
        glm::vec2 v3 = glm::vec2(transform_ * glm::vec4(bounds.right(), bounds.top(), 0.0f, 1.0f));
        glm::vec2 v4 = glm::vec2(transform_ * glm::vec4(bounds.left(), bounds.top(), 0.0f, 1.0f));
        glm::vec2 v5 = glm::vec2(Renderer::raycast(base_layer));
        float bounded_area = math::shoelace({v1, v2, v3, v4});
        if (bounded_area < math::shoelace({v1, v2, v3, v4, v5}))
        {
            return false;
        }
        if (bounded_area < math::shoelace({v1, v2, v3, v5, v4}))
        {
            return false;
        }
        if (bounded_area < math::shoelace({v1, v2, v5, v3, v4}))
        {
            return false;
        }
        if (bounded_area < math::shoelace({v1, v5, v2, v3, v4}))
        {
            return false;
        }
        if (bounded_area < math::shoelace({v5, v1, v2, v3, v4}))
        {
            return false;
        }
        return true;
    }
    return false;
}
shader_data_type Widget::get_value() { return value; }
void Widget::set_value(shader_data_type value) { this->value = value; }
void Widget::set_layer(Layer *layer) { base_layer = layer; }
Layer *Widget::get_layer() { return base_layer; }
} // namespace mare

#endif