#ifndef BUTTON
#define BUTTON

// MARE
#include "Components/RenderPack.hpp"
#include "Components/Widget.hpp"
#include "Entity.hpp"
#include "Materials/BasicColorMaterial.hpp"
#include "Meshes/CharMesh.hpp"
#include "Meshes/QuadrangleMesh.hpp"
#include "Systems/Rendering/PacketRenderer.hpp"

namespace mare {

// forward declare controls
template <typename T> class ButtonControls;
// declare button template callback type
template <typename T> using on_click_callback = void (*)(T *);

/**
 * @brief A Button UI element for executing a process or toggling a setting when
 * clicked.
 *
 */
template <typename T> class Button : public Widget<bool>, public RenderPack {
public:
  Button(Layer *layer, util::Rect widget_bounds, std::string label,
         Referenced<Mesh> icon = nullptr)
      : Widget(layer, widget_bounds), icon_mesh(icon) {
    value = false;
    button_box = gen_ref<QuadrangleMesh>();
    if (!label.empty()) {
      label_mesh = gen_ref<CharMesh>(label, 0.08f);
    } else {
      label_mesh = nullptr;
    }

    box_material = gen_ref<BasicColorMaterial>();
    label_material = gen_ref<BasicColorMaterial>();
    box_material->set_color(box_color);
    label_material->set_color(label_color);

    rescale();

    push_packet({button_box, box_material});
    if (label_mesh) {
      push_packet({label_mesh, label_material});
    }
    if (icon_mesh) {
      push_packet({icon_mesh, label_material});
    }

    gen_system<PacketRenderer>();
    gen_system<ButtonControls<T>>();
  }
  void rescale() override {
    float text_scale = (bounds.top() - bounds.bottom()) / 1.5f;
    if (label_mesh) {
      label_mesh->set_scale(glm::vec3(text_scale, text_scale, 1.0f));
      label_mesh->set_center(util::get_rect_center(bounds));
    }
    if (icon_mesh) {
      icon_mesh->set_scale(glm::vec3(text_scale, text_scale, 1.0f));
      auto center = util::get_rect_center(bounds);
      icon_mesh->set_position({center.x, center.y, 0.0f});
    }
    float box_scale_x = bounds.right() - bounds.left();
    float box_scale_y = bounds.top() - bounds.bottom();
    button_box->set_scale({box_scale_x, box_scale_y, 1.0f});
    glm::vec2 rect_center = util::get_rect_center(bounds);
    button_box->set_position({rect_center.x, rect_center.y, 1.0f});
  }
  /**
   * @brief Set the on click callback of the Button.
   * @details The on click callback will be executed each time the button is
   * pressed.
   *
   * @param callback_func The function to set as a callback for the Button.
   */
  void set_on_click_callback(on_click_callback<T> callback_func,
                             T *callback_entity) {
    callback_entity_ = callback_entity;
    on_click_func = callback_func;
  }
  /**
   * @brief Executes a user defined function when the Button is clicked. The
   * boolean value of the Button Widget is also negated when the Button is
   * pressed.
   *
   */
  void on_click() {
    if (on_click_func && callback_entity_) {
      value = !value;
      on_click_func(callback_entity_);
    }
  }
  Referenced<QuadrangleMesh> button_box;
  Referenced<CharMesh> label_mesh;
  Referenced<Mesh> icon_mesh;
  Referenced<BasicColorMaterial> box_material;
  Referenced<BasicColorMaterial> label_material;
  glm::vec4 box_color{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec4 box_pushed_color{0.17f, 0.45f, 1.0f, 1.0f};
  glm::vec4 label_color{0.0f, 0.0f, 0.0f, 1.0f};
  glm::vec4 label_pushed_color{1.0f, 1.0f, 1.0f, 1.0f};

private:
  on_click_callback<T> on_click_func = nullptr;
  T *callback_entity_ = nullptr;
};

template <typename T> class ButtonControls : public ControlsSystem<Button<T>> {
public:
  bool on_mouse_button(const RendererInput &input, Button<T> *button) override {
    if (input.LEFT_MOUSE_JUST_PRESSED && button->is_cursor_in_bounds()) {
      UIElement::focus(button);
      button->box_material->set_color(button->box_pushed_color);
      button->label_material->set_color(button->label_pushed_color);
      return false;
    }
    if(input.LEFT_MOUSE_JUST_RELEASED && button->is_cursor_in_bounds())
    {
      button->box_material->set_color(button->box_color);
      button->label_material->set_color(button->label_color);
      button->on_click();
      return true;
    }
    button->box_material->set_color(button->box_color);
    button->label_material->set_color(button->label_color);
    return false;
  }
};

} // namespace mare

#endif