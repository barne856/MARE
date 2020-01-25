#ifndef SLIDERUI
#define SLIDERUI

// MARE
#include "mare/Widget.hpp"
#include "mare/Meshes/QuadrangleMesh.hpp"
#include "mare/InstancedMesh.hpp"
#include "mare/Materials/BasicMaterial.hpp"

namespace mare
{
class SliderUI : public Widget
{
public:
    SliderUI(Layer* layer) : Widget(layer), quad_mesh(nullptr), slider_mesh(nullptr), solid_material(nullptr), color(glm::vec4(1.0f))
    {
        // create object
        quad_mesh = new QuadrangleMesh();
        slider_mesh = new InstancedMesh(2);
        slider_mesh->set_mesh(quad_mesh);
        slider_mesh->push_instance(glm::translate(glm::mat4(1.0f), {-0.5f, 0.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)));
        slider_mesh->push_instance(glm::scale(glm::mat4(1.0f), {1.0f, 0.01f, 0.0f}));
        solid_material = new BasicMaterial();
        solid_material->bind();
        solid_material->set_color(color);
        push_primative(slider_mesh, solid_material);
        // set value
        value = 0.0f;
        // set bounds
        bounds.left() = -0.55f;
        bounds.right() = 0.55f;
        bounds.top() = 0.05f;
        bounds.bottom() = -0.05f;
    }
    bool on_mouse_button(const RendererInput &input) override
    {
        if (input.mouse_button == 1 && is_in_bounds(input.mouse_pos))
        {
            Application::set_focus(this);
            return on_mouse_move(input);
        }
        Application::set_focus(nullptr);
        return false;
    }
    bool on_mouse_move(const RendererInput &input)
    {
        if (input.focus == this)
        {
            glm::vec2 relative_position = get_widget_coords(input.mouse_pos);
            float x = glm::clamp(relative_position.x, -0.5f, 0.5f);
            slider_mesh->update_instance(0, glm::translate(glm::mat4(1.0f), {x, 0.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)));
            value = x + 0.5f;
            // event is handled
            return true;
        }
        return false;
    }

private:
    QuadrangleMesh *quad_mesh;
    InstancedMesh *slider_mesh;
    BasicMaterial *solid_material;
    glm::vec4 color;
};
} // namespace mare

#endif