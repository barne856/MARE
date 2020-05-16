#ifndef SLIDERUI
#define SLIDERUI

// MARE
#include "mare/Components/Widget.hpp"
#include "mare/Assets/Meshes/QuadrangleMesh.hpp"
#include "mare/Meshes.hpp"
#include "mare/Assets/Materials/BasicMaterial.hpp"
#include "mare/Renderer.hpp"
#include "mare/Systems.hpp"
#include "mare/Entity.hpp"

namespace mare
{

// forward declare SliderUI components
class SliderUIRenderer;
class SliderUIControls;

class SliderUI : public Entity, public Widget
{
public:
    SliderUI(Layer* layer) : Widget(layer), quad_mesh(nullptr), slider_mesh(nullptr), solid_material(nullptr), color(glm::vec4(1.0f))
    {
        // create object
        quad_mesh = gen_scoped<QuadrangleMesh>();
        slider_mesh = gen_ref<InstancedMesh>(2);
        slider_mesh->set_mesh(std::move(quad_mesh));
        slider_mesh->push_instance(glm::translate(glm::mat4(1.0f), {-0.5f, 0.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)));
        slider_mesh->push_instance(glm::scale(glm::mat4(1.0f), {1.0f, 0.01f, 0.0f}));
        solid_material = gen_ref<BasicMaterial>();
        solid_material->bind();
        solid_material->set_color(color);
        // set bounds
        bounds.left() = -0.55f;
        bounds.right() = 0.55f;
        bounds.top() = 0.05f;
        bounds.bottom() = -0.05f;

        // set initial position
        glm::ivec2 screen_size = glm::ivec2(Renderer::get_info().window_width, Renderer::get_info().window_height);
        glm::vec3 world_size = Renderer::raycast(get_layer(), screen_size);
        set_position({-world_size.x + 0.6f, -0.9f, 0.0f});

        // push components
        gen_system<SliderUIRenderer>();
        gen_system<SliderUIControls>();
    }
    void set_color(glm::vec4 color)
    {
        solid_material->set_color(color);
    }
    void set_value(shader_data_type value) override
    {
        this->value = value;
        (*slider_mesh)[0] = glm::translate(glm::mat4(1.0f), {std::get<float>(value) - 0.5f, 0.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
    }
    Scoped<QuadrangleMesh> quad_mesh;
    Referenced<InstancedMesh> slider_mesh;
    Referenced<BasicMaterial> solid_material;
    glm::vec4 color;
};

class SliderUIRenderer : public RenderSystem<SliderUI>
{
public:
    void render(float dt, Camera *camera, SliderUI *slider_ui) override
    {
        slider_ui->solid_material->bind();
        slider_ui->solid_material->render();
        slider_ui->slider_mesh->render(camera, slider_ui->solid_material.get(), slider_ui->get_model());
    }
};

class SliderUIControls : public ControlsSystem<SliderUI>
{
public:
    bool on_mouse_button(const RendererInput &input, SliderUI *slider_ui)
    {
        if (input.mouse_button == 1 && slider_ui->is_in_bounds())
        {
            Renderer::get_info().focus = slider_ui->get_layer();
            return on_mouse_move(input, slider_ui);
        }
        Renderer::get_info().focus = nullptr;
        return false;
    }
    bool on_mouse_move(const RendererInput &input, SliderUI *slider_ui)
    {
        if (Renderer::get_info().focus == slider_ui->get_layer())
        {
            glm::vec2 relative_position = slider_ui->get_widget_coords();
            float x = glm::clamp(relative_position.x, -0.5f, 0.5f);
            (*(slider_ui->slider_mesh))[0] = glm::translate(glm::mat4(1.0f), {x, 0.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
            slider_ui->set_value(x + 0.5f);
            // event is handled
            return true;
        }
        return false;
    }
    bool on_resize(const RendererInput &input, SliderUI *slider_ui)
    {
        // position widgets on the screen
        glm::ivec2 screen_size = glm::ivec2(Renderer::get_info().window_width, Renderer::get_info().window_height);
        glm::vec3 world_size = Renderer::raycast(slider_ui->get_layer(), screen_size);
        slider_ui->set_position({-world_size.x + 0.6f, -0.9f, 0.0f});
        return false;
    }
    bool on_key(const RendererInput &input, SliderUI *slider_ui) { return false; }
    bool on_mouse_wheel(const RendererInput &input, SliderUI *slider_ui) { return false; }
};

} // namespace mare

#endif