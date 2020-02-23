// MARE
#include "mare/Renderer.hpp"
#include "mare/Widget.hpp"

namespace mare
{
Widget::Widget(Layer *layer) : base_layer(layer), value({}), bounds({}) {}
Widget::~Widget() {}
glm::vec2 Widget::get_widget_coords()
{
    return glm::vec2(glm::inverse(transform_) * glm::vec4(Renderer::API->raycast(base_layer->get_camera()), 1.0f));
}
bool Widget::is_in_bounds()
{
    glm::vec2 v1 = glm::vec2(transform_ * glm::vec4(bounds.left(), bounds.bottom(), 0.0f, 1.0f));
    glm::vec2 v2 = glm::vec2(transform_ * glm::vec4(bounds.right(), bounds.bottom(), 0.0f, 1.0f));
    glm::vec2 v3 = glm::vec2(transform_ * glm::vec4(bounds.right(), bounds.top(), 0.0f, 1.0f));
    glm::vec2 v4 = glm::vec2(transform_ * glm::vec4(bounds.left(), bounds.top(), 0.0f, 1.0f));
    glm::vec2 v5 = glm::vec2(Renderer::API->raycast(base_layer->get_camera()));
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
shader_data_type Widget::get_value() { return value; }
void Widget::set_value(shader_data_type value) { this->value = value; }
} // namespace mare