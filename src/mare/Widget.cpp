// MARE
#include "mare/Widget.hpp"

namespace mare
{
Widget::Widget(Layer *layer) : base_layer(layer), value({}), bounds({})
{
    set_camera(base_layer->get_camera());
}
Widget::~Widget()
{
    // all meshes and materials from the object are cleaned up on delete automatically by Object base class
    // set camera to nullptr, it will be deleted by the layer it was created from
    set_camera(nullptr);
}
glm::vec2 Widget::get_widget_coords(glm::ivec2 screen_coords)
{
    return glm::vec2(glm::inverse(transform) * glm::vec4(get_camera()->screen_to_world(screen_coords), 1.0f));
}
bool Widget::is_in_bounds(glm::ivec2 screen_coords)
{
    glm::vec2 v1 = glm::vec2(transform * glm::vec4(bounds.left(), bounds.bottom(), 0.0f, 1.0f));
    glm::vec2 v2 = glm::vec2(transform * glm::vec4(bounds.right(), bounds.bottom(), 0.0f, 1.0f));
    glm::vec2 v3 = glm::vec2(transform * glm::vec4(bounds.right(), bounds.top(), 0.0f, 1.0f));
    glm::vec2 v4 = glm::vec2(transform * glm::vec4(bounds.left(), bounds.top(), 0.0f, 1.0f));
    glm::vec2 v5 = glm::vec2(get_camera()->screen_to_world(screen_coords));
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