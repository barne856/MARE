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

using widget_value = std::variant<int, float, bool, glm::vec2, glm::vec3, glm::vec4, glm::mat2, glm::mat3, glm::mat4>;

class Widget : public Layer, public Object
{
public:
    using Object::render;
    Widget(Layer *layer) : base_layer(layer)
    {
        set_camera(base_layer->get_camera());
    }
    virtual ~Widget()
    {
        // all meshes and materials from the object are cleaned up on delete automatically by Object base class
        // set camera to nullptr, it will be deleted by the layer it was created from
        set_camera(nullptr);
    }
    glm::vec2 get_widget_coords(glm::ivec2 screen_coords)
    {
        return glm::vec2(glm::inverse(transform) * glm::vec4(get_camera()->screen_to_world(screen_coords), 1.0f));
    }
    bool is_in_bounds(glm::ivec2 screen_coords)
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
    widget_value get_value() { return value; }

protected:
    widget_value value{};
    Rect bounds{};
    Layer *base_layer;
};
} // namespace mare

#endif