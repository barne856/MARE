// MARE
#include "mare/Renderer.hpp"
#include "mare/Widget.hpp"

namespace mare
{
Widget::Widget(Layer* layer) : value({}), bounds({}), base_layer(layer) {}
Widget::~Widget() {}
glm::vec2 Widget::get_widget_coords()
{
    if (base_layer)
    {
        return glm::vec2(glm::inverse(transform_) * glm::vec4(Renderer::API->raycast(base_layer), 1.0f));
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
        glm::vec2 v5 = glm::vec2(Renderer::API->raycast(base_layer));
        float bounded_area = shoelace({v1, v2, v3, v4});
        if (bounded_area < shoelace({v1, v2, v3, v4, v5}))
        {
            return false;
        }
        if (bounded_area < shoelace({v1, v2, v3, v5, v4}))
        {
            return false;
        }
        if (bounded_area < shoelace({v1, v2, v5, v3, v4}))
        {
            return false;
        }
        if (bounded_area < shoelace({v1, v5, v2, v3, v4}))
        {
            return false;
        }
        if (bounded_area < shoelace({v5, v1, v2, v3, v4}))
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