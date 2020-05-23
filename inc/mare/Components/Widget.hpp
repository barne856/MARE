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

namespace util {
/**
 * @brief A simple struct to define a bounding box of a UI element.
 */
struct Rect {
  float &left() { return m_values[0]; }
  float &bottom() { return m_values[1]; }
  float &right() { return m_values[2]; }
  float &top() { return m_values[3]; }

  void set_rect(glm::vec2 bottom_left, glm::vec2 top_right) {
    m_values = glm::vec4(bottom_left, top_right);
  }

  float operator[](unsigned i) const { return m_values[i]; }
  float &operator[](unsigned i) { return m_values[i]; }

private:
  glm::vec4 m_values{};
};
} // namespace util
using namespace util;
/**
 * @brief A Component used to create basic UI elements.
 * @details A Widget is a 2D Entity that allows the user to create derived
 * classes to implement simple UI elements. Widgets contain a value of type <T>
 * that can be get and set. Normally the user will add a ControlsSystem and a
 * RenderSystem to the widget to control and render it on a Layer in a Scene.
 *
 * @tparam <T> The type of value the widget holds.
 */
template <typename T> class Widget : virtual public Transform {
public:
  /**
   * @brief Construct a new Widget object
   *
   * @param layer The Layer the Widget will be rendered on.
   */
  Widget(Layer *layer) : value{}, bounds{}, base_layer(layer) {}
  virtual ~Widget() {}

  /**
   * @brief Get the widget coordinates in the base Layer's coordinate system.
   *
   * @return The position of the Widget in the Layer's coordinates.
   */
  glm::vec2 get_widget_coords() {
    if (base_layer) {
      return glm::vec2(glm::inverse(get_model()) *
                       glm::vec4(Renderer::raycast(base_layer), 1.0f));
    }
    return glm::vec2(0.0f, 0.0f);
  }
  /**
   * @brief Test if the mouse is inside the bounds of the Widget.
   *
   * @return true
   * @return false
   */
  bool is_in_bounds() {
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
  virtual void set_value(T value) { this->value = value; }
  /**
   * @brief Set the base Layer of the Widget.
   *
   * @param layer A pointer to the Layer to set as the base Layer for the
   * Widget.
   */
  void set_layer(Layer *layer) { base_layer = layer; }
  /**
   * @brief Get the base Layer of the Widget.
   *
   * @return A pointer to the base Layer of the Widget.
   */
  Layer *get_layer() { return base_layer; }

protected:
  T value;     /**< The Widget's value.*/
  Rect bounds; /**< The bounds of the Widget in the base Layer's coordinates.
                  Used to test if the mouse position is in bounds.*/
  Layer *
      base_layer; /**< A pointer to the Layer which the Widget is attached to.*/
};
} // namespace mare

#endif