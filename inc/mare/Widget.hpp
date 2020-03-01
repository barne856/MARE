#ifndef WIDGET
#define WIDGET

// MARE
#include "mare/Entity.hpp"
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

class Widget : public Entity
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
} // namespace mare

#endif