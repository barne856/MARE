#ifndef DROPDOWN
#define DROPDOWN

// TODO:
// animate scrolling and dropdown
// display "..." if names are too long
// scroll bar

// MARE
#include "Components/RenderPack.hpp"
#include "Components/Widget.hpp"
#include "Materials/BasicColorMaterial.hpp"
#include "Meshes/CharMesh.hpp"
#include "Meshes/QuadrangleMesh.hpp"
#include "Meshes/TriangleMesh.hpp"
#include "Systems/Rendering/PacketRenderer.hpp"

// Standard Library
#include <string>
#include <vector>

namespace mare {
// forward declare systems
class DropdownControls;
class DropdownListControls;
class DropdownListPhysics;
class DropdownListRenderer;
class Dropdown;

/**
 * @brief The list part of the Dropdown Widget. This is pushed onto the entity
 * stack when the dropdown list is opened by clicking the dropdown arrow on the
 * dropdown box.
 *
 */
class DropdownList : public Widget<std::vector<std::string>> {
public:
  DropdownList(Layer *layer, util::Rect widget_bounds,
               Dropdown *parent_dropdown_box)
      : Widget(layer, widget_bounds) {
    dropdown_box = parent_dropdown_box;

    box_material = gen_ref<BasicColorMaterial>();
    box_material->set_color(box_color);
    text_material = gen_ref<BasicColorMaterial>();
    text_material->set_color(text_color);
    selection_material = gen_ref<BasicColorMaterial>();
    selection_material->set_color(selection_color);

    rescale();

    gen_system<DropdownListControls>();
    // gen_system<DropdownListPhysics>();
    gen_system<DropdownListRenderer>();
  }
  void set_selection_options(std::vector<std::string> options,
                             uint32_t max_options_shown) {
    value = options;
    max_rows =
        std::min(max_options_shown, static_cast<uint32_t>(options.size()));
    dropdown_boxes.clear();
    dropdown_string_meshes.clear();
    for (auto &str : options) {
      dropdown_boxes.push_back(gen_ref<QuadrangleMesh>());
      dropdown_string_meshes.push_back(gen_ref<CharMesh>(str, 0.08f));
    }
    scroll_pos = 0;
    selection = -1;
  }
  void update_selection(glm::vec2 parent_coords) {
    selection = static_cast<uint32_t>(static_cast<float>(max_rows) *
                                      (bounds.top() - parent_coords.y) /
                                      (bounds.top() - bounds.bottom())) +
                scroll_pos;
  }
  float get_unit_height() {
    return (bounds.top() - bounds.bottom()) / static_cast<float>(max_rows);
  }
  float get_unit_width() { return bounds.right() - bounds.left(); }
  glm::vec2 get_center() {
    return glm::vec2(bounds.left() + get_unit_width() / 2.0f,
                     bounds.top() - bounds.bottom());
  }
  void rescale() override {
    float unit_height =
        (bounds.top() - bounds.bottom()) / static_cast<float>(max_rows);
    float boarder_thickness = 0.15f * unit_height;
    float width = bounds.right() - bounds.left();
    glm::vec2 center =
        glm::vec2(bounds.left() + width / 2.0f, bounds.top() - bounds.bottom());
    for (int i = 0; i < scroll_pos; i++) {
      dropdown_boxes[i]->set_scale({width, unit_height, 1.0f});
      dropdown_boxes[i]->set_position(
          {center.x, bounds.top() - unit_height * 0.5f, 0.0f});
      dropdown_string_meshes[i]->set_scale(
          glm::vec3(unit_height - 2.0f * boarder_thickness));
      dropdown_string_meshes[i]->set_position(
          {bounds.left() + boarder_thickness, bounds.top() - boarder_thickness,
           0.0f});
    }
    for (int i = scroll_pos; i < max_rows + scroll_pos; i++) {
      dropdown_boxes[i]->set_scale({width, unit_height, 1.0f});
      dropdown_boxes[i]->set_position(
          {center.x,
           bounds.top() -
               unit_height * (static_cast<float>(i - scroll_pos) + 0.5f),
           0.0f});
      dropdown_string_meshes[i]->set_scale(
          glm::vec3(unit_height - 2.0f * boarder_thickness));
      dropdown_string_meshes[i]->set_position(
          {bounds.left() + boarder_thickness,
           bounds.top() - unit_height * static_cast<float>(i - scroll_pos) -
               boarder_thickness,
           0.0f});
    }
    for (int i = max_rows + scroll_pos; i < value.size(); i++) {
      dropdown_boxes[i]->set_scale({0.0f, 0.0f, 1.0f});
      dropdown_boxes[i]->set_position(
          {center.x,
           bounds.top() -
               unit_height * (static_cast<float>(max_rows + 1) + 0.5f),
           0.0f});
      dropdown_string_meshes[i]->set_scale(glm::vec3(0.0f));
      dropdown_string_meshes[i]->set_position(
          {bounds.left() + boarder_thickness,
           bounds.top() - unit_height * static_cast<float>(max_rows + 1) -
               boarder_thickness,
           0.0f});
    }
  }
  void on_focus() override {}
  void on_unfocus() override {}
  std::vector<Referenced<QuadrangleMesh>> dropdown_boxes;
  std::vector<Referenced<CharMesh>> dropdown_string_meshes;
  Referenced<BasicColorMaterial> box_material;
  Referenced<BasicColorMaterial> text_material;
  Referenced<BasicColorMaterial> selection_material;
  glm::vec4 box_color{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec4 text_color{0.0f, 0.0f, 0.0f, 1.0f};
  glm::vec4 selection_color{0.17f, 0.45f, 1.0f, 1.0f};
  bool opened = false;
  uint32_t max_rows = 0;
  uint32_t scroll_pos = 0;
  int selection = -1;
  Dropdown *dropdown_box;
};

/**
 * @brief A Dropdown UI element to select an option from a list of options.
 *
 */
class Dropdown : public Widget<std::string>, public RenderPack {
public:
  Dropdown(Layer *layer, util::Rect widget_bounds, std::string selection_hint)
      : Widget(layer, widget_bounds) {
    value = "";
    selection_hint_ = selection_hint;

    dropdown_list = gen_ref<DropdownList>(layer, util::Rect(), this);

    value_box = gen_ref<QuadrangleMesh>();
    dropdown_arrow_box = gen_ref<QuadrangleMesh>();
    dropdown_arrow = gen_ref<TriangleMesh>(
        glm::vec2(0.0f, -0.5f), glm::vec2(0.5f, 0.5f), glm::vec2(-0.5f, 0.5f));
    value_string_mesh =
        gen_ref<CharMesh>(selection_hint + ": " + value, 0.08f, 0.0f, 1000);
    box_material = gen_ref<BasicColorMaterial>();
    box_material->set_color(box_color);
    text_material = gen_ref<BasicColorMaterial>();
    text_material->set_color(text_color);
    dropdown_arrow_box_material = gen_ref<BasicColorMaterial>();
    dropdown_arrow_box_material->set_color(accent_color);

    rescale();

    push_packet({value_box, box_material});
    push_packet({dropdown_arrow_box, dropdown_arrow_box_material});
    push_packet({dropdown_arrow, box_material});
    push_packet({value_string_mesh, text_material});

    gen_system<DropdownControls>();
    gen_system<PacketRenderer>();
  }
  void set_selection_options(std::vector<std::string> options,
                             uint32_t max_rows) {
    dropdown_list->set_selection_options(options, max_rows);
    set_value("");
    value_string_mesh->set_text(selection_hint_ + ": " + value);
    rescale();
  }
  void rescale() override {
    float boarder_thickness = 0.15f * (bounds.top() - bounds.bottom());
    float height = bounds.top() - bounds.bottom();
    float value_box_width = bounds.right() - bounds.left() - height;
    value_box->set_scale({value_box_width, height, 1.0f});
    value_box->set_position({bounds.left() + value_box_width / 2.0f,
                             bounds.bottom() + height / 2.0f, 0.0f});
    dropdown_arrow_box->set_scale({height, height, 1.0f});
    dropdown_arrow_box->set_position({bounds.right() - height / 2.0f,
                                      bounds.bottom() + height / 2.0f, 0.0f});
    dropdown_arrow->set_scale({height / 2.0f, height / 2.0f, 1.0f});
    dropdown_arrow->set_position({bounds.right() - height / 2.0f,
                                  bounds.bottom() + height / 2.0f, 0.0f});
    value_string_mesh->set_scale(glm::vec3(height - 2.0f * boarder_thickness));
    value_string_mesh->set_position({bounds.left() + boarder_thickness,
                                     bounds.top() - boarder_thickness, 0.0f});
    util::Rect list_bounds = util::Rect();
    list_bounds.top() = bounds.bottom();
    list_bounds.left() = bounds.left();
    list_bounds.right() = bounds.right() - height;
    list_bounds.bottom() = list_bounds.top() -
                           static_cast<float>(dropdown_list->max_rows) * height;
    dropdown_list->set_bounds(list_bounds);
  }
  bool is_cursor_in_list_bounds() {
    glm::vec2 position = get_model_coords();
    if (position.x >= dropdown_list->get_left() &&
        position.x <= dropdown_list->get_right() &&
        position.y <= dropdown_list->get_top() &&
        position.y >= dropdown_list->get_bottom()) {
      return true;
    }
    return false;
  }
  bool update_selection() {
    glm::vec2 position = get_model_coords();
    if (is_cursor_in_list_bounds()) {
      dropdown_list->update_selection(position);
      set_value(dropdown_list->get_value()[dropdown_list->selection]);
      value_string_mesh->set_text(selection_hint_ + ": " + value);
      rescale();
      on_unfocus();
      return true;
    }
    return false;
  }
  void on_focus() override {}
  void on_unfocus() override {
    if (dropdown_list->opened) {
      get_layer()->pull_entity<DropdownList>(dropdown_list.get());
      dropdown_list->opened = false;
    }
  }
  Referenced<QuadrangleMesh> value_box;
  Referenced<QuadrangleMesh> dropdown_arrow_box;
  Referenced<TriangleMesh> dropdown_arrow;
  Referenced<CharMesh> value_string_mesh;
  Referenced<BasicColorMaterial> box_material;
  Referenced<BasicColorMaterial> dropdown_arrow_box_material;
  Referenced<BasicColorMaterial> text_material;
  glm::vec4 box_color{1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec4 text_color{0.0f, 0.0f, 0.0f, 1.0f};
  glm::vec4 accent_color{0.17f, 0.45f, 1.0f, 1.0f};
  Referenced<DropdownList> dropdown_list;
  std::string selection_hint_;
};

class DropdownControls : public ControlsSystem<Dropdown> {
public:
  bool on_mouse_button(const RendererInput &input,
                       Dropdown *dropdown) override {
    if (input.LEFT_MOUSE_JUST_RELEASED && dropdown->is_cursor_in_bounds() &&
        dropdown->dropdown_list->opened == false) {
      dropdown->get_layer()->push_entity(dropdown->dropdown_list);
      dropdown->dropdown_list->opened = true;
      UIElement::focus(dropdown);
      return true;
    }
    if (!dropdown->is_cursor_in_bounds() &&
        !dropdown->is_cursor_in_list_bounds() &&
        dropdown->dropdown_list->opened == true) {
      dropdown->get_layer()->pull_entity<DropdownList>(
          dropdown->dropdown_list.get());
      dropdown->dropdown_list->opened = false;
      return false;
    }
    return false;
  }
};

class DropdownListControls : public ControlsSystem<DropdownList> {
public:
  bool on_mouse_button(const RendererInput &input,
                       DropdownList *dropdown_list) override {
    if (input.LEFT_MOUSE_JUST_RELEASED) {
      return dropdown_list->dropdown_box->update_selection();
    }
    if (input.LEFT_MOUSE_JUST_PRESSED &&
        dropdown_list->dropdown_box->is_cursor_in_list_bounds()) {
      return true;
    }
    dropdown_list->dropdown_box->on_unfocus();
    return false;
  }
  bool on_key(const RendererInput &input,
              DropdownList *dropdown_list) override {
    if (dropdown_list->opened) {
      if (input.UP_JUST_PRESSED) {
        dropdown_list->scroll_pos =
            std::max(static_cast<int>(dropdown_list->scroll_pos) - 1, 0);
        dropdown_list->rescale();
      }
      if (input.DOWN_JUST_PRESSED) {
        dropdown_list->scroll_pos = std::min(
            dropdown_list->scroll_pos + 1,
            static_cast<unsigned int>(dropdown_list->get_value().size() -
                                      dropdown_list->max_rows));
        dropdown_list->rescale();
      }
    }
    return false;
  }
  bool on_mouse_wheel(const RendererInput &input,
                      DropdownList *dropdown_list) override {
    if (dropdown_list->opened &&
        dropdown_list->dropdown_box->is_cursor_in_list_bounds()) {
      if (input.mouse_scroll == 1) {
        dropdown_list->scroll_pos =
            std::max(static_cast<int>(dropdown_list->scroll_pos) - 1, 0);
        dropdown_list->rescale();
        return true;
      }
      if (input.mouse_scroll == -1) {
        dropdown_list->scroll_pos = std::min(
            dropdown_list->scroll_pos + 1,
            static_cast<unsigned int>(dropdown_list->get_value().size() -
                                      dropdown_list->max_rows));
        dropdown_list->rescale();
        return true;
      }
    }
    return false;
  }
};

class DropdownListPhysics : public PhysicsSystem<DropdownList> {
public:
  void update(float dt, DropdownList *dropdown_list) override {
    float unit_height = dropdown_list->get_unit_height();
    float width = dropdown_list->get_unit_width();
    glm::vec2 center = dropdown_list->get_center();
    // move any item above the scroll position to the top of the stack.
    for (int i = 0; i < dropdown_list->scroll_pos; i++) {
      glm::vec3 scale = dropdown_list->dropdown_boxes[i]->get_scale();
      if (scale.x != width || scale.y != unit_height) {
        scale.x = std::min(scale.x + scale_speed * dt * width, width);
        scale.y =
            std::min(scale.y + scale_speed * dt * unit_height, unit_height);
        dropdown_list->dropdown_boxes[i]->set_scale(scale);
        dropdown_list->dropdown_string_meshes[i]->set_scale(glm::vec3(scale.y));
      }
      glm::vec3 position = dropdown_list->dropdown_boxes[i]->get_position();
      if (position.y != dropdown_list->get_top() - unit_height * 0.5f ||
          position.x != center.x) {
        position.x = center.x;
        position.y = std::min(position.y + translate_speed * dt,
                              dropdown_list->get_top() - unit_height * 0.5f);
      }
      dropdown_list->dropdown_boxes[i]->set_position(position);
      dropdown_list->dropdown_string_meshes[i]->set_center(glm::vec2(position));
    }
    // move items that are shown under or at the scroll position
    for (int i = dropdown_list->scroll_pos;
         i < dropdown_list->scroll_pos + dropdown_list->max_rows; i++) {
      glm::vec3 scale = dropdown_list->dropdown_boxes[i]->get_scale();
      if (scale.x != width || scale.y != unit_height) {
        scale.x = std::min(scale.x + scale_speed * dt * width, width);
        scale.y =
            std::min(scale.y + scale_speed * dt * unit_height, unit_height);
        dropdown_list->dropdown_boxes[i]->set_scale(scale);
        dropdown_list->dropdown_string_meshes[i]->set_scale(glm::vec3(scale.y));
      }
      glm::vec3 position = dropdown_list->dropdown_boxes[i]->get_position();
      float lerp_to =
          dropdown_list->get_top() -
          unit_height *
              (static_cast<float>(i - dropdown_list->scroll_pos) + 0.5f);
      if (position.y != lerp_to || position.x != center.x) {
        position.x = center.x;
        if (position.y < lerp_to) {
          position.y = std::min(position.y + translate_speed * dt, lerp_to);
        } else if (position.y > lerp_to) {
          position.y = std::max(position.y - translate_speed * dt, lerp_to);
        }
      }
      dropdown_list->dropdown_boxes[i]->set_position(position);
      dropdown_list->dropdown_string_meshes[i]->set_center(glm::vec2(position));
    }
    // move items that are not shown after the max rows
    for (int i = dropdown_list->scroll_pos + dropdown_list->max_rows;
         i < dropdown_list->get_value().size(); i++) {
      glm::vec3 scale = dropdown_list->dropdown_boxes[i]->get_scale();
      if (scale.x != 0 || scale.y != 0) {
        scale.x = std::max(scale.x - scale_speed * dt * width, 0.0f);
        scale.y = std::max(scale.y - scale_speed * dt * unit_height, 0.0f);
        dropdown_list->dropdown_boxes[i]->set_scale(scale);
        dropdown_list->dropdown_string_meshes[i]->set_scale(glm::vec3(scale.y));
      }
      glm::vec3 position = dropdown_list->dropdown_boxes[i]->get_position();
      if (position.y != dropdown_list->get_top() -
                            unit_height * (dropdown_list->max_rows + 0.5f) ||
          position.x != center.x) {
        position.x = center.x;
        position.y =
            std::min(position.y + translate_speed * dt,
                     dropdown_list->get_top() -
                         unit_height * (dropdown_list->max_rows + 0.5f));
      }
      dropdown_list->dropdown_boxes[i]->set_position(position);
      dropdown_list->dropdown_string_meshes[i]->set_center(glm::vec2(position));
    }
  }
  float scale_speed = 10.0f;
  float translate_speed = 1.0f;
};

class DropdownListRenderer : public RenderSystem<DropdownList> {
public:
  void render(float dt, Camera *camera, DropdownList *dropdown_list) override {
    for (int i = 0; i < dropdown_list->get_value().size(); i++) {
      if (i == dropdown_list->selection) {
        dropdown_list->dropdown_boxes[i]->render(
            camera, dropdown_list->selection_material.get(),
            dropdown_list->dropdown_box);
      } else {
        dropdown_list->dropdown_boxes[i]->render(
            camera, dropdown_list->box_material.get(),
            dropdown_list->dropdown_box);
      }
      dropdown_list->dropdown_string_meshes[i]->render(
          camera, dropdown_list->text_material.get(),
          dropdown_list->dropdown_box);
    }
  }
};

} // namespace mare

#endif