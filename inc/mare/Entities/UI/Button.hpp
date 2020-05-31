#ifndef BUTTON
#define BUTTON

// MARE
#include "mare/Assets/Materials/BasicMaterial.hpp"
#include "mare/Assets/Meshes/CharMesh.hpp"
#include "mare/Assets/Meshes/QuadrangleMesh.hpp"
#include "mare/Components/RenderPack.hpp"
#include "mare/Components/Widget.hpp"
#include "mare/Entity.hpp"
#include "mare/Systems/Rendering/PacketRenderer.hpp"

namespace mare {

// forward declare controls
class ButtonControls;
// typedef callback
typedef void (*on_click_callback)(Layer*);

/**
 * @brief A Button UI element for executing a process or toggling a setting when
 * clicked.
 *
 */
class Button : public Entity, public Widget<bool>, public RenderPack {
public:
  Button(Layer *layer, util::Rect widget_bounds, std::string label)
      : Widget(layer) {
    bounds = widget_bounds;
    button_box = gen_ref<QuadrangleMesh>();
    button_boarder = gen_ref<QuadrangleMesh>();
    label_mesh = gen_ref<CharMesh>(label);
    box_material = gen_ref<BasicMaterial>();
    boarder_material = gen_ref<BasicMaterial>();
    label_material = gen_ref<BasicMaterial>();
    box_material->set_color(box_color);
    boarder_material->set_color(boarder_color);
    label_material->set_color(label_color);

    rescale();

    push_packet({button_boarder, boarder_material});
    push_packet({button_box, box_material});
    push_packet({label_mesh, boarder_material});

    gen_system<PacketRenderer>();
    gen_system<ButtonControls>();
  }
  void rescale() {
    float text_scale = (bounds.top() - bounds.bottom()) / 1.5f;
    label_mesh->set_scale(glm::vec3(text_scale * 0.5f, text_scale, 1.0f));
    label_mesh->set_center(util::get_rect_center(bounds));
    float boarder_thickness = 0.01f * (bounds.right() - bounds.left());
    float box_scale_x = bounds.right() - bounds.left();
    float box_scale_y = bounds.top() - bounds.bottom();
    float boarder_scale_x = box_scale_x + boarder_thickness;
    float boarder_scale_y = box_scale_y + boarder_thickness;
    button_boarder->set_scale({boarder_scale_x, boarder_scale_y, 1.0f});
    button_box->set_scale({box_scale_x, box_scale_y, 1.0f});
    glm::vec2 rect_center = util::get_rect_center(bounds);
    button_boarder->set_position({rect_center.x, rect_center.y, 1.0f});
    button_box->set_position({rect_center.x, rect_center.y, 1.0f});
  }
  void set_on_click_callback(on_click_callback callback_func) {
    on_click_func = callback_func;
  }
  void on_click() {
    if (on_click_func) {
      on_click_func(get_layer());
    }
  }
  Referenced<QuadrangleMesh> button_box;
  Referenced<QuadrangleMesh> button_boarder;
  Referenced<CharMesh> label_mesh;
  Referenced<BasicMaterial> box_material;
  Referenced<BasicMaterial> boarder_material;
  Referenced<BasicMaterial> label_material;
  glm::vec4 boarder_color{0.0f, 0.0f, 0.0f, 1.0f};
  glm::vec4 box_color{0.85f, 0.85f, 0.85f, 1.0f};
  glm::vec4 box_pushed_color{0.5f, 0.5f, 0.5f, 1.0f};
  glm::vec4 label_color{0.0f, 0.0f, 0.0f, 1.0f};

private:
  on_click_callback on_click_func = nullptr;
};

class ButtonControls : public ControlsSystem<Button> {
public:
  bool on_mouse_button(const RendererInput &input, Button *button) override {
    if (input.LEFT_MOUSE_JUST_PRESSED && button->is_cursor_in_bounds()) {
      UIElement::focus(button);
      button->box_material->set_color(button->box_pushed_color);
      button->on_click();
      return false;
    }
    button->box_material->set_color(button->box_color);
    return false;
  }
};

} // namespace mare

#endif