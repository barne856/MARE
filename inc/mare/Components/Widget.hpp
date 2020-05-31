#ifndef WIDGET
#define WIDGET

// MARE
#include "mare/Layer.hpp"
#include "mare/Mare.hpp"

// External Libraries
#include "glm.hpp"

// Standar Library
#include <variant>

namespace mare {
using namespace util;

class UIElement : virtual public Transform {
public:
  UIElement(Layer *layer) : bounds{}, base_layer(layer) {}
  virtual ~UIElement() {}
  /**
   * @brief Test if the cursor is inside the bounds of the UIElement.
   *
   * @return true
   * @return false
   */
  bool is_cursor_in_bounds() {
    if (base_layer) {
      glm::vec2 v1 = glm::vec2(
          get_model() * glm::vec4(bounds.left(), bounds.bottom(), 0.0f, 1.0f));
      glm::vec2 v2 = glm::vec2(
          get_model() * glm::vec4(bounds.right(), bounds.bottom(), 0.0f, 1.0f));
      glm::vec2 v3 = glm::vec2(
          get_model() * glm::vec4(bounds.right(), bounds.top(), 0.0f, 1.0f));
      glm::vec2 v4 = glm::vec2(
          get_model() * glm::vec4(bounds.left(), bounds.top(), 0.0f, 1.0f));
      glm::vec2 v5 = glm::vec2(Renderer::raycast(base_layer));
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
      return glm::vec2(glm::inverse(get_model()) *
                       glm::vec4(Renderer::raycast(base_layer), 1.0f));
    }
    return glm::vec2(0.0f, 0.0f);
  }
  bool is_focused()
  {
    return (Renderer::get_info().focused_element == this);
  }
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

  Rect bounds; /**< The bounds of the UIElement in the base UIElement's model
                  space. Used to test if the mouse position is in bounds.*/
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
  Widget(Layer *layer) : UIElement(layer), value{} {}
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