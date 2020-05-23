#ifndef TRANSFORM
#define TRANSFORM

// MARE
#include "mare/Mare.hpp"

// External Libraries
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

namespace mare {
/**
 * @brief A Transform Component that provides the data and functions to
 * transform and Entity in space.
 * @details This Component contains the model matrix of an Entity and provides a
 * set of function to transform Entities.
 */
class Transform : virtual public Component {
public:
  /**
   * @brief Construct a new Transform object
   *
   */
  Transform()
      : translation_(glm::vec3(0.0f)), rotation_(glm::mat4(1.0f)),
        scale_(glm::vec3(1.0f)), transform_(glm::mat4(1.0f)),
        normal_(glm::mat3(1.0f)), up_vector_(glm::vec3(0.0f, 1.0f, 0.0f)),
        forward_vector_(glm::vec3(0.0f, 0.0f, -1.0f)) {}
  /**
   * @brief Destroy the Transform object
   *
   */
  virtual ~Transform() {}
  /**
   * @brief Move the Transform from its current position by an offsetting vec3.
   *
   * @param translation The vector to translate by.
   */
  void translate(glm::vec3 translation) {
    translation_ += translation;
    update_model_matrix();
  }
  /**
   * @brief Set the position of the Transform by absolute world space
   * coordinates.
   *
   * @param position The new position of the Transform.
   */
  void set_position(glm::vec3 position) {
    translation_ = position;
    update_model_matrix();
  }
  /**
   * @brief Rotate the Transform by an offsetting angle about an axis.
   *
   * @param axis The axis to rotate about.
   * @param angle The angle to rotate by.
   */
  void rotate(glm::vec3 axis, float angle) {
    rotation_ = glm::rotate(rotation_, angle, axis);
    update_model_matrix();
  }
  /**
   * @brief Set the absolute rotation of the Transform matrix.
   *
   * @param axis The axis to rotate about.
   * @param angle The angle to rotate by.
   */
  void set_rotation(glm::vec3 axis, float angle) {
    rotation_ = glm::rotate(glm::mat4(1.0f), angle, axis);
    update_model_matrix();
  }
  /**
   * @brief Set the absolute rotation of the Transform matrix.
   *
   * @param rotation The rotation matix to set.
   */
  void set_rotation(glm::mat3 rotation) { rotation_ = rotation; }
  /**
   * @brief Set the absolute scale of the Transform matrix.
   *
   * @param scale
   */
  void set_scale(glm::vec3 scale) {
    scale_ = scale;
    update_model_matrix();
  }
  /**
   * @brief Set the up vector of the Transform.
   * @details This is used to calculate the view matrix and can only be changed
   * through this function.
   * @param up The up direction of the Transform's view.
   */
  void set_up_vector(glm::vec3 up) { up_vector_ = up; }
  /**
   * @brief Set the forward vector Transform
   * @details This is used to calculate the view matrix and can only be changed
   * through this function.
   * @param forward The forward direction of the Transform's view.
   */
  void set_forward_vector(glm::vec3 forward) { forward_vector_ = forward; }
  /**
   * @brief Face the Transform towards a direction in space.
   *
   * @param direction The direction to face.
   */
  void face_towards(glm::vec3 direction) {
    direction = glm::normalize(direction);
    glm::vec3 axis = glm::cross(forward_vector_, direction);
    float angle =
        acosf(glm::dot(forward_vector_, direction) /
              (glm::length(forward_vector_) * glm::length(direction)));
    set_rotation(axis, angle);
  }
  /**
   * @brief Make the Transform look at a point in space.
   * @details This will orient the Transform such that the forward vector is
   * pointed towards a point in space.
   *
   * @param center The point to look at.
   */
  void look_at(glm::vec3 center) { face_towards(center - translation_); }
  /**
   * @brief Get the view matrix of the Transform.
   *
   * @return The view matrix of the Transform.
   */
  glm::mat4 get_view() const {
    return glm::lookAt(translation_,
                       translation_ + glm::mat3(rotation_) * forward_vector_,
                       up_vector_);
  }
  /**
   * @brief Get the direction the Transform is pointed towards.
   *
   * @return The direction the Transform is pointer towards.
   */
  glm::vec3 get_direction() const {
    return glm::mat3(rotation_) * forward_vector_;
  }
  /**
   * @brief Get the up vector of the Transform's view.
   *
   * @return The up vector.
   */
  glm::vec3 get_up_vector() const { return up_vector_; }
  /**
   * @brief Get the forward vector of the Transform's view.
   *
   * @return The forward vector.
   */
  glm::vec3 get_forward_vector() const { return forward_vector_; }
  /**
   * @brief Get the position of the Transform in world space.
   *
   * @return The position of the Transform.
   */
  inline glm::vec3 get_position() const { return translation_; }
  /**
   * @brief Get the scale of the Transform.
   *
   * @return The scale of the Transform.
   */
  inline glm::vec3 get_scale() const { return scale_; }
  /**
   * @brief Get the model matrix of the Transform.
   *
   * @return The model matrix.
   */
  inline glm::mat4 get_model() const { return transform_; }
  /**
   * @brief Get the normal matrix of the Transform.
   *
   * @return The normal matrix of the Transform.
   */
  inline glm::mat3 get_normal() const { return normal_; }
  /**
   * @brief Get the rotation matrix of the Transform.
   *
   * @return The rotation matrix.
   */
  inline glm::mat4 get_rotation() const { return rotation_; }
  /**
   * @brief Function to swap the data in two Transform Components.
   * @details Swaps all of the data in the two Transforms effectively swapping
   * the actual Components.
   * @param transform A Referenced Transform to swap data with.
   */
  void swap_transform(Referenced<Transform> transform) {
    glm::vec3 translation_temp = translation_;
    glm::mat4 rotation_temp = rotation_;
    glm::vec3 scale_temp = scale_;
    glm::vec3 up_vector_temp = up_vector_;
    glm::vec3 forward_vector_temp = forward_vector_;

    set_position(transform->get_position());
    set_rotation(transform->get_rotation());
    set_scale(transform->get_scale());
    set_up_vector(transform->get_up_vector());
    set_forward_vector(transform->get_forward_vector());

    transform->set_position(translation_temp);
    transform->set_rotation(rotation_temp);
    transform->set_scale(scale_);
    transform->set_up_vector(up_vector_temp);
    transform->set_forward_vector(forward_vector_temp);
  }

private:
  /**
   * @brief Function to update the model matrix after an operation is performed
   * on the Transform.
   */
  void update_model_matrix() {
    transform_ = glm::translate(glm::mat4(1.0f), translation_) * rotation_ *
                 glm::scale(glm::mat4(1.0f), scale_);
    normal_ = glm::transpose(glm::inverse(
        glm::mat3(rotation_ * glm::scale(glm::mat4(1.0f), scale_))));
  }

  glm::vec3 translation_; /**< The position in world space of the Transform.*/
  glm::mat4 rotation_;    /**< The rotation matrix of the Transform.*/
  glm::vec3 scale_;       /**< The scale of the Transform.*/
  glm::mat4 transform_;   /**< The full model matrix of the Transform.*/
  glm::mat3 normal_;      /**< The normal matrix of the Transform.*/
  glm::vec3 up_vector_;   /**< The up vector of the Transform, used to calculate
                             the view.*/
  glm::vec3 forward_vector_; /**< The forward vector of the Transform, used to
                              calculate the view.*/
};

} // namespace mare

#endif