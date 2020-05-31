#ifndef SLIDERUI
#define SLIDERUI

// MARE
#include "mare/Assets/Materials/BasicMaterial.hpp"
#include "mare/Assets/Meshes/QuadrangleMesh.hpp"
#include "mare/Components/Widget.hpp"
#include "mare/Entity.hpp"
#include "mare/Meshes.hpp"
#include "mare/Renderer.hpp"
#include "mare/Systems.hpp"

namespace mare {

// forward declare SliderUI components
class SliderUIRenderer;
class SliderUIControls;

/**
 * @brief A simple UI element that provides a slider bar and contains a `float`
 * value.
 */
class SliderUI : public Entity, public Widget<float> {
public:
  /**
   * @brief Construct a new SliderUI object.
   *
   * @param layer The base Layer that this UI element is on.
   */
  SliderUI(Layer *layer)
      : Widget(layer), quad_mesh(nullptr), slider_mesh(nullptr),
        solid_material(nullptr), color(glm::vec4(1.0f)) {
    // create object
    quad_mesh = gen_scoped<QuadrangleMesh>();
    slider_mesh = gen_ref<InstancedMesh>(2);
    slider_mesh->set_mesh(std::move(quad_mesh));
    slider_mesh->push_instance(
        glm::translate(glm::mat4(1.0f), {-0.5f, 0.0f, 0.0f}) *
        glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)));
    slider_mesh->push_instance(
        glm::scale(glm::mat4(1.0f), {1.0f, 0.01f, 0.0f}));
    solid_material = gen_ref<BasicMaterial>();
    solid_material->bind();
    solid_material->set_color(color);
    // set bounds
    bounds.left() = -0.55f;
    bounds.right() = 0.55f;
    bounds.top() = 0.05f;
    bounds.bottom() = -0.05f;
    // push components
    gen_system<SliderUIRenderer>();
    gen_system<SliderUIControls>();
  }
  /**
   * @brief Set the color of the slider bar element.
   *
   * @param color The color to set.
   */
  void set_color(glm::vec4 color) { solid_material->set_color(color); }
  Scoped<QuadrangleMesh> quad_mesh; /**< The base mesh for the instanced mesh.*/
  Referenced<InstancedMesh> slider_mesh; /**< The slider mesh.*/
  Referenced<BasicMaterial>
      solid_material; /**< The material used to render the slider.*/
  glm::vec4 color;    /**< The color of the slider.*/
};

/**
 * @brief A System used to render the SliderUI
 */
class SliderUIRenderer : public RenderSystem<SliderUI> {
public:
  void render(float dt, Camera *camera, SliderUI *slider_ui) override {
    slider_ui->solid_material->bind();
    slider_ui->solid_material->render();
    slider_ui->slider_mesh->render(camera, slider_ui->solid_material.get(),
                                   slider_ui->get_model());
  }
};

/**
 * @brief A System used to control the SliderUI
 */
class SliderUIControls : public ControlsSystem<SliderUI> {
public:
  /**
   * @brief If the left mouse button is pressed when pointed at the slider, set
   * the focus to the base Layer and move the knob to the pointed location which
   * will change the value of the SliderUI Widget.
   *
   * @param input The RendererInput from the Engine.
   * @param slider_ui The SliderUI.
   * @return true event is handeled.
   * @return false pass on event.
   */
  bool on_mouse_button(const RendererInput &input, SliderUI *slider_ui) {
    if (input.LEFT_MOUSE_JUST_PRESSED && slider_ui->is_cursor_in_bounds()) {
      UIElement::focus(slider_ui);
      return on_mouse_move(input, slider_ui);
    }
    return false;
  }
  /**
   * @brief Move the slider knob to the mouse position and adjust the Widget's
   * value if the base Layer is focused.
   *
   * @param input The RendererInput from the Engine.
   * @param slider_ui The SliderUI.
   * @return true event is handeled.
   * @return false pass on event.
   */
  bool on_mouse_move(const RendererInput &input, SliderUI *slider_ui) {
    if (slider_ui->is_focused()) {
      glm::vec2 relative_position = slider_ui->get_model_coords();
      float x = glm::clamp(relative_position.x, -0.5f, 0.5f);
      (*(slider_ui->slider_mesh))[0] =
        glm::translate(glm::mat4(1.0f), {x, 0.0f, 0.0f}) *
        glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
      slider_ui->set_value(x + 0.5f);
      // event is handled
      return true;
    }
    return false;
  }
};

} // namespace mare

#endif