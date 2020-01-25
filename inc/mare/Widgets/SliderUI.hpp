#ifndef SLIDERUI
#define SLIDERUI

// MARE
#include "mare/Widget.hpp"
#include "mare/Meshes/QuadrangleMesh.hpp"
#include "mare/InstancedMesh.hpp"
#include "mare/Materials/BasicMaterial.hpp"

#include <iostream>

namespace mare
{
class SliderUI : public Widget
{
public:
    SliderUI(Camera *camera) : quad_mesh(nullptr), slider_mesh(nullptr), solid_material(nullptr), color(glm::vec4(1.0f))
    {
        set_camera(camera);
        quad_mesh = new QuadrangleMesh();
        slider_mesh = new InstancedMesh(2);
        slider_mesh->set_mesh(quad_mesh);
        slider_mesh->push_instance(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)));
        slider_mesh->push_instance(glm::scale(glm::mat4(1.0f), {1.0f, 0.01f, 0.0f}));

        solid_material = new BasicMaterial();
        solid_material->bind();
        solid_material->set_color(color);

        push_primative(slider_mesh, solid_material);
    }
    ~SliderUI()
    {
        // all meshes and materials are cleaned up on delete automatically
        // set camera to nullptr, it will be deleted by the layer if was created from
        set_camera(nullptr);
    }
    void set_scale(glm::vec3 scale)
    {
        primatives[0].mesh->set_scale(scale);
    }
    void set_position(glm::vec3 position)
    {
        primatives[0].mesh->set_position(position);
    }
    bool on_mouse_button(const RendererInput &input) override
    {
        if (input.mouse_button == 1 && is_in_bounds(get_camera()->screen_to_world(input.mouse_pos)))
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
            // get world space coordinates
            glm::vec3 world_position = get_camera()->screen_to_world(input.mouse_pos);
            float left = -0.5f * primatives[0].mesh->get_scale().x + primatives[0].mesh->get_position().x;
            float right = 0.5f * primatives[0].mesh->get_scale().x + primatives[0].mesh->get_position().x;
            float x = (world_position.x - left) / (right - left) - 0.5f;
            x = glm::clamp(x, -0.5f, 0.5f);
            slider_mesh->update_instance(0, glm::translate(glm::mat4(1.0f), {x, 0.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)));
            // event is handled
            return true;
        }
        return false;
    }
    bool is_in_bounds(glm::vec3 world_position)
    {
        float left = -0.5f * primatives[0].mesh->get_scale().x + primatives[0].mesh->get_position().x;
        float right = 0.5f * primatives[0].mesh->get_scale().x + primatives[0].mesh->get_position().x;
        float top = 0.1f * primatives[0].mesh->get_scale().y + primatives[0].mesh->get_position().y;
        float bottom = -0.1f * primatives[0].mesh->get_scale().y + primatives[0].mesh->get_position().y;
        if (world_position.x > left && world_position.x < right && world_position.y < top && world_position.y > bottom)
        {
            return true;
        }
        return false;
    }

private:
    QuadrangleMesh *quad_mesh;
    InstancedMesh *slider_mesh;
    BasicMaterial *solid_material;
    glm::vec4 color;
}; // namespace mare
} // namespace mare

#endif