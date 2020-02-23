#ifndef SLIDERUI
#define SLIDERUI

// MARE
#include "mare/Widget.hpp"
#include "mare/Meshes/QuadrangleMesh.hpp"
#include "mare/InstancedMesh.hpp"
#include "mare/Materials/BasicMaterial.hpp"
#include "mare/Renderer.hpp"

namespace mare
{
class SliderUI : public Widget
{
public:
    SliderUI(Layer* layer) : Widget(layer), quad_mesh(nullptr), slider_mesh(nullptr), solid_material(nullptr), color(glm::vec4(1.0f))
    {
        // create object
        quad_mesh = Renderer::API->GenScoped<QuadrangleMesh>();
        slider_mesh = Renderer::API->GenRef<InstancedMesh>("slider_mesh", 2);
        slider_mesh->set_mesh(std::move(quad_mesh));
        slider_mesh->push_instance(glm::translate(glm::mat4(1.0f), {-0.5f, 0.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)));
        slider_mesh->push_instance(glm::scale(glm::mat4(1.0f), {1.0f, 0.01f, 0.0f}));
        solid_material = Renderer::API->GenRef<BasicMaterial>("slider_material");
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
        // remove meshes from global access
        Renderer::API->DeleteRef("slider_mesh");
        Renderer::API->DeleteRef("slider_material");
    }
    bool on_mouse_button(const RendererInput &input) override
    {
        if (input.mouse_button == 1 && is_in_bounds())
        {
            Renderer::API->get_info().focus = this;
            return on_mouse_move(input);
        }
        Renderer::API->get_info().focus = nullptr;
        return false;
    }
    bool on_mouse_move(const RendererInput &input)
    {
        if (Renderer::API->get_info().focus == this)
        {
            glm::vec2 relative_position = get_widget_coords();
            float x = glm::clamp(relative_position.x, -0.5f, 0.5f);
            (*slider_mesh)[0] = glm::translate(glm::mat4(1.0f), {x, 0.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
            value = x + 0.5f;
            // event is handled
            return true;
        }
        return false;
    }
    void set_value(shader_data_type value) override
    {
        this->value = value;
        (*slider_mesh)[0] = glm::translate(glm::mat4(1.0f), {std::get<float>(value) - 0.5f, 0.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
    }

private:
    Scoped<QuadrangleMesh> quad_mesh;
    Referenced<InstancedMesh> slider_mesh;
    Referenced<BasicMaterial> solid_material;
    glm::vec4 color;
};
} // namespace mare

#endif