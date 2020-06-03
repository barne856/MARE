#ifndef SLIDER
#define SLIDER

// MARE
#include "mare/Assets/Materials/BasicMaterial.hpp"
#include "mare/Assets/Meshes/CircleMesh.hpp"
#include "mare/Assets/Meshes/QuadrangleMesh.hpp"
#include "mare/Components/RenderPack.hpp"
#include "mare/Components/Widget.hpp"
#include "mare/Entity.hpp"
#include "mare/Meshes.hpp"
#include "mare/Renderer.hpp"
#include "mare/Systems.hpp"
#include "mare/Systems/Rendering/PacketRenderer.hpp"

namespace mare {

// forward declare Slider components
class SliderRenderer;
class SliderControls;

/**
 * @brief A simple UI element that provides a slider bar and contains a `float`
 * value.
 */
class Slider : public Entity, public Widget<float>, public RenderPack {
public:
  /**
   * @brief Construct a new SliderUI object.
   *
   * @param layer The base Layer that this UI element is on.
   */
  Slider(Layer *layer, util::Rect widget_bounds)
      : Widget(layer, widget_bounds) {
    value = 0.0f;

    fill_mesh = gen_ref<QuadrangleMesh>();
    bar_mesh = gen_ref<QuadrangleMesh>();
    knob_mesh = gen_ref<CircleMesh>(32, 0.5f);
    left_circle_mesh = gen_ref<CircleMesh>(32, 0.5f);
    right_circle_mesh = gen_ref<CircleMesh>(32, 0.5f);

    slider_material = gen_ref<BasicMaterial>();
    fill_material = gen_ref<BasicMaterial>();
    knob_material = gen_ref<BasicMaterial>();
    slider_material->set_color(slider_color);
    fill_material->set_color(fill_color);
    knob_material->set_color(knob_color);

    // use bounds to set the scale
    rescale();

    push_packet({left_circle_mesh, fill_material});
    push_packet({right_circle_mesh, slider_material});
    push_packet({fill_mesh, fill_material});
    push_packet({bar_mesh, slider_material});
    push_packet({knob_mesh, knob_material});

    // push systems
    gen_system<PacketRenderer>();
    gen_system<SliderControls>();
  }
  void rescale() override {
    float height = bounds.top() - bounds.bottom();
    float left = bounds.left() + 0.5f * height;
    float right = bounds.right() - 0.5f * height;

    left_circle_mesh->set_scale(glm::vec3(height / 1.5f));
    left_circle_mesh->set_position({left, 0.0f, 0.0f});
    right_circle_mesh->set_scale(glm::vec3(height / 1.5f));
    right_circle_mesh->set_position({right, 0.0f, 0.0f});
    knob_mesh->set_scale(glm::vec3(height));
    knob_mesh->set_position({left + value * (right - left), 0.0f, 0.0f});
    bar_mesh->set_scale({(1.0f - value) * (right - left), height / 1.5f, 1.0f});
    fill_mesh->set_scale({value * (right - left), height / 1.5f, 1.0f});

    glm::vec2 center = util::get_rect_center(bounds);
    left_circle_mesh->translate({center.x, center.y, 0.0f});
    right_circle_mesh->translate({center.x, center.y, 0.0f});
    knob_mesh->translate({center.x, center.y, 0.0f});
    fill_mesh->set_position(
        {left + 0.5f * value * (right - left) + center.x, center.y, 0.0f});
    bar_mesh->set_position(
        {right - 0.5f * (1.0f - value) * (right - left) + center.x, center.y,
         0.0f});
  }
  /**
   * @brief Convert the horizontal coordinate in model space to a value for the
   * slider.
   *
   * @param x The horizontal coordinate of position in the Slider's model space.
   * @return The value.
   */
  float position_to_value(float x) {
    float height = bounds.top() - bounds.bottom();
    float left = bounds.left() + 0.5f * height;
    float right = bounds.right() - 0.5f * height;
    return glm::clamp((x - left) / (right - left), 0.0f, 1.0f);
  }
  Referenced<QuadrangleMesh> fill_mesh;
  Referenced<QuadrangleMesh> bar_mesh;
  Referenced<CircleMesh> knob_mesh;
  Referenced<CircleMesh> left_circle_mesh;
  Referenced<CircleMesh> right_circle_mesh;
  Referenced<BasicMaterial>
      slider_material; /**< The material used to render the slider background
                          and knob.*/
  Referenced<BasicMaterial>
      fill_material; /**< The material used to render the slider background.*/
  Referenced<BasicMaterial> knob_material;
  glm::vec4 slider_color{
      1.0f, 1.0f, 1.0f,
      1.0f}; /**< The color of the bacground and knob for the slider.*/
  glm::vec4 fill_color{0.25f, 0.3f, 0.9f,
                       1.0f}; /**< The color of the fill for the slider.*/
  glm::vec4 knob_color{0.77f, 0.77f, 0.85f, 1.0f};
};

/**
 * @brief A System used to control the SliderUI
 */
class SliderControls : public ControlsSystem<Slider> {
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
  bool on_mouse_button(const RendererInput &input, Slider *slider_ui) {
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
  bool on_mouse_move(const RendererInput &input, Slider *slider_ui) {
    if (slider_ui->is_focused()) {
      glm::vec2 relative_position = slider_ui->get_model_coords();
      slider_ui->set_value(slider_ui->position_to_value(relative_position.x));
      slider_ui->rescale();
      // event is handled
      return true;
    }
    return false;
  }
};

} // namespace mare

#endif