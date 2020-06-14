#ifndef WIDGET
#define WIDGET

// MARE
#include "Layer.hpp"
#include "Mare.hpp"
#include "Entity.hpp"

// External Libraries
#include "glm.hpp"

// Standar Library
#include <variant>

namespace mare {
using namespace util;

class UIElement : virtual public Entity {
public:
  /**
   * @brief Abstract interface for a UIElement.
   *
   * @param layer The base Layer where the UIElement is attached.
   * @param bounds The bounds of the UIElement use to scale and position the
   * UIElement on the Layer and test for user input. This applies a
   * transformation to the element in addition to any transformation defined by
   * the Transform of the UIElement.
   */
  UIElement(Layer *layer, Rect bounds) : bounds{bounds}, base_layer(layer) {}
  virtual ~UIElement() {}
  /**
   * @brief Test if the cursor is inside the bounds of the UIElement.
   *
   * @return true
   * @return false
   */
  bool is_cursor_in_bounds() {
    if (base_layer) {
      glm::vec2 v1 =
          glm::vec2(get_transformation_matrix() *
                    glm::vec4(bounds.left(), bounds.bottom(), 0.0f, 1.0f));
      glm::vec2 v2 =
          glm::vec2(get_transformation_matrix() *
                    glm::vec4(bounds.right(), bounds.bottom(), 0.0f, 1.0f));
      glm::vec2 v3 =
          glm::vec2(get_transformation_matrix() *
                    glm::vec4(bounds.right(), bounds.top(), 0.0f, 1.0f));
      glm::vec2 v4 =
          glm::vec2(get_transformation_matrix() *
                    glm::vec4(bounds.left(), bounds.top(), 0.0f, 1.0f));
      glm::vec2 v5{};
      if (base_layer->get_type() == ProjectionType::ORTHOGRAPHIC) {
        float x = base_layer->get_ortho_scale() * base_layer->get_aspect() *
                  (2.0f * (float)Renderer::get_input().mouse_pos.x /
                       (float)(Renderer::get_info().window_width) -
                   1.0f);
        float y = base_layer->get_ortho_scale() *
                  (-2.0f * (float)Renderer::get_input().mouse_pos.y /
                       (float)(Renderer::get_info().window_height) +
                   1.0f);
        v5 = {x, y};
      } else {
        v5 = glm::vec2(Renderer::raycast(base_layer));
      }
      float bounded_area = math::shoelace({v1, v2, v3, v4});
      if (bounded_area < math::shoelace({v1, v2, v3, v4, v5})) {
        return false;
      }
      if (bounded_area < math::shoelace({v1, v2, v3, v5, v4})) {
        return false;
      }
      if (bounded_area < math::shoelace({v1, v2, v5, v3, v4})) {
        return false;
      }
      if (bounded_area < math::shoelace({v1, v5, v2, v3, v4})) {
        return false;
      }
      if (bounded_area < math::shoelace({v5, v1, v2, v3, v4})) {
        return false;
      }
      return true;
    }
    return false;
  }
  /**
   * @brief Get the coordinates of the cursor in the UIElement's model space.
   *
   * @return The position of the cursor in the UIElement's model space
   * coordinates.
   */
  glm::vec2 get_model_coords() {
    if (base_layer) {
      if (base_layer->get_type() == ProjectionType::ORTHOGRAPHIC) {
        float x = base_layer->get_ortho_scale() * base_layer->get_aspect() *
                  (2.0f * (float)Renderer::get_input().mouse_pos.x /
                       (float)(Renderer::get_info().window_width) -
                   1.0f);
        float y = base_layer->get_ortho_scale() *
                  (-2.0f * (float)Renderer::get_input().mouse_pos.y /
                       (float)(Renderer::get_info().window_height) +
                   1.0f);
        return glm::vec2(glm::inverse(get_transformation_matrix()) *
                         glm::vec4(x, y, 0.0f, 1.0f));
      }
      return glm::vec2(glm::inverse(get_transformation_matrix()) *
                       glm::vec4(Renderer::raycast(base_layer), 1.0f));
    }
    return glm::vec2(0.0f, 0.0f);
  }
  bool is_focused() { return (Renderer::get_info().focused_element == this); }
  virtual void on_focus() {}
  virtual void on_unfocus() {}
  static void focus(UIElement *element) {
    if (Renderer::get_info().focused_element) {
      Renderer::get_info().focused_element->on_unfocus();
    }
    Renderer::get_info().focused_element = element;
    if (element) {
      element->on_focus();
    }
  }
  Layer *get_layer() { return base_layer; }
  /**
   * @brief Used to rescale the UIElement based on the bounds.
   *
   */
  virtual void rescale() = 0;
  void set_left(float value) {
    bounds.left() = value;
    rescale();
  }
  void set_bottom(float value) {
    bounds.bottom() = value;
    rescale();
  }
  void set_right(float value) {
    bounds.right() = value;
    rescale();
  }
  void set_top(float value) {
    bounds.top() = value;
    rescale();
  }
  void set_bounds(Rect new_bounds)
  {
    bounds.left() = new_bounds.left();
    bounds.right() = new_bounds.right();
    bounds.top() = new_bounds.top();
    bounds.bottom() = new_bounds.bottom();
    rescale();
  }
  float get_left() { return bounds.left(); }
  float get_right() { return bounds.right(); }
  float get_top() { return bounds.top(); }
  float get_bottom() { return bounds.bottom(); }

protected:
  Rect bounds; /**< The bounds of the UIElement in the base UIElement's model
space. Used to test if the mouse position is in bounds. The bounds is used as a
position an scale in addition to the Transform of the UIElement.*/

private:
  Layer *base_layer; /**< A pointer to the Layer which the UIElement is attached
                        to.*/
};

/**
 * @brief A Component used to create basic UI elements.
 * @details A Widget is a 2D Entity that allows the user to create derived
 * classes to implement simple UI elements. Widgets contain a value of type <T>
 * that can be get and set. Normally the user will add a ControlsSystem and a
 * RenderSystem to the widget to control and render it on a Layer in a Scene.
 *
 * @tparam <T> The type of value the widget holds.
 */
template <typename T> class Widget : public UIElement {
public:
  /**
   * @brief Construct a new Widget object
   *
   * @param layer The Layer the Widget will be rendered on.
   */
  Widget(Layer *layer, Rect bounds) : UIElement(layer, bounds), value{} {}
  virtual ~Widget() {}
  /**
   * @brief Get the value of the Widget.
   *
   * @return <T> The Widgets current value.
   */
  T get_value() { return value; }
  /**
   * @brief Set the value of the Widget.
   *
   * @param value The value to set.
   */
  void set_value(T value) { this->value = value; }

protected:
  T value; /**< The Widget's value.*/
};
} // namespace mare

#endif