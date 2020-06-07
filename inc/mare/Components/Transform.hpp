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
 * transform an Entity in space.
 * @details The Transform is the base class for all Components and consists of a
 * single glm::mat4 variable that is a TRS matrix or Translation * Rotation *
 * Scale matrix. This means that the order of transformations applied is Scale
 * then Rotate the Translate.
 *
 * Components are used to add functionality to Entities through data
 * and function members. All Components inherit virtually from Transform. When
 * creating Entities, inherit from Components to add the required functionality.
 * Components are operated on by Systems.
 * @see System
 * @see Entity
 */
class Transform {
public:
  /**
   * @brief Construct a new Transform object
   *
   */
  Transform() : transformation_matrix_(glm::mat4(1.0f)) {}
  Transform(glm::mat4 transform_matrix)
      : transformation_matrix_(transform_matrix) {}
  /**
   * @brief Get the position of the Transform.
   *
   * @return The position.
   */
  glm::vec3 get_position() const {
    return glm::vec3(transformation_matrix_[3]);
  }
  /**
   * @brief Get the scale of the Transform.
   *
   * @return The scale.
   */
  glm::vec3 get_scale() const {
    glm::vec3 scale_vector{};
    scale_vector.x = glm::length(transformation_matrix_[0]);
    scale_vector.y = glm::length(transformation_matrix_[1]);
    scale_vector.z = glm::length(transformation_matrix_[2]);
    return scale_vector;
  }
  /**
   * @brief Get the translation matrix of the Transform.
   *
   * @return The translation matrix
   */
  glm::mat4 get_translation_matrix() const {
    return glm::translate(glm::mat4(1.0f),
                          glm::vec3(transformation_matrix_[3]));
  }
  /**
   * @brief Get the rotation matrix of the Transform.
   *
   * @return The rotation matrix
   */
  glm::mat4 get_rotation_matrix() const {
    glm::vec3 scale = get_scale();
    glm::mat4 rotation = glm::mat4(1.0f);
    if (scale.x != 0.0f) {
      rotation[0][0] = transformation_matrix_[0][0] / scale.x;
      rotation[0][1] = transformation_matrix_[0][1] / scale.x;
      rotation[0][2] = transformation_matrix_[0][2] / scale.x;
    }
    if (scale.y != 0.0f) {
      rotation[1][0] = transformation_matrix_[1][0] / scale.y;
      rotation[1][1] = transformation_matrix_[1][1] / scale.y;
      rotation[1][2] = transformation_matrix_[1][2] / scale.y;
    }
    if (scale.z != 0.0f) {
      rotation[2][0] = transformation_matrix_[2][0] / scale.z;
      rotation[2][1] = transformation_matrix_[2][1] / scale.z;
      rotation[2][2] = transformation_matrix_[2][2] / scale.z;
    }
    return rotation;
  }
  /**
   * @brief Get the scale matrix of the Transform.
   *
   * @return The scale matrix.
   */
  glm::mat4 get_scale_matrix() const {
    return glm::scale(glm::mat4(1.0f), get_scale());
  }
  /**
   * @brief Get the transformation matrix.
   *
   * @return The transformation matrix.
   */
  glm::mat4 get_transformation_matrix() { return transformation_matrix_; }
  /**
   * @brief Set the transformation matrix of the Transform
   *
   * @param transformation_matrix The matrix to set.
   */
  void set_transformation_matrix(glm::mat4 transformation_matrix) {
    transformation_matrix_ = transformation_matrix;
  }
  /**
   * @brief Get the normal matrix of the Transform used to transform normals in
   * shaders.
   *
   * @return The normal matrix.
   */
  glm::mat3 get_normal_matrix() const {
    return glm::transpose(glm::inverse(glm::mat3(transformation_matrix_)));
  }
  /**
   * @brief Get the view matrix of the Transform used by Cameras.
   *
   * @return The view matrix of the Transform.
   */
  glm::mat4 get_view_matrix() const {
    return glm::inverse(transformation_matrix_);
  }
  /**
   * @brief Face the Transform towards a point in space and orient it such that
   * the horizontal plane is level.
   *
   * @param point The point in space to face towards.
   * @param normal_plane The normal vector for the horizontal plane.
   */
  void face_towards(glm::vec3 point, glm::vec3 normal_vector) {
    glm::mat4 view = glm::lookAt(get_position(), point, normal_vector);
    transformation_matrix_ = glm::inverse(view) * get_scale_matrix();
  }
  /**
   * @brief Translate the transform by an offsetting 3D vector.
   *
   * @param offset The 3D vector to translate by.
   */
  void translate(glm::vec3 offset) {
    transformation_matrix_[3][0] += offset.x;
    transformation_matrix_[3][1] += offset.y;
    transformation_matrix_[3][2] += offset.z;
  }
  /**
   * @brief Set the position of the Transform.
   *
   * @param position The position to set.
   */
  void set_position(glm::vec3 position) {
    transformation_matrix_[3][0] = position.x;
    transformation_matrix_[3][1] = position.y;
    transformation_matrix_[3][2] = position.z;
  }
  /**
   * @brief Rotate the Transform from its current rotation by an angle about an
   * axis.
   *
   * @param axis The axis to rotate about.
   * @param angle The angle to rotate by.
   */
  void rotate(glm::vec3 axis, float angle) {
    glm::mat4 rotation =
        glm::rotate(glm::mat4(1.0f), angle, axis) * get_rotation_matrix();
    transformation_matrix_ =
        get_translation_matrix() * rotation * get_scale_matrix();
  }
  /**
   * @brief Set the rotation of the Transform.
   *
   * @param axis The axis to rotate about.
   * @param angle The angle to rotate by.
   */
  void set_rotation(glm::vec3 axis, float angle) {
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angle, axis);
    transformation_matrix_ =
        get_translation_matrix() * rotation * get_scale_matrix();
  }
  void set_rotation_matrix(glm::mat4 rotation_matrix) {
    transformation_matrix_ =
        get_translation_matrix() * rotation_matrix * get_scale_matrix();
  }
  /**
   * @brief Set the scale of the Transform
   *
   * @param scale The scale to set.
   */
  void set_scale(glm::vec3 scale) {
    glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), scale);
    transformation_matrix_ =
        get_translation_matrix() * get_rotation_matrix() * scale_matrix;
  }
  /**
   * @brief Get the forward vector of the Transform.
   *
   * @return The forward vector.
   */
  glm::vec3 get_forward_vector() const {
    glm::mat4 rotation_matrix = get_rotation_matrix();
    return -glm::vec3(rotation_matrix[2]);
  }
  /**
   * @brief Get the right vector of the Transform.
   *
   * @return The right vector.
   */
  glm::vec3 get_right_vector() const {
    glm::mat4 rotation_matrix = get_rotation_matrix();
    return glm::vec3(rotation_matrix[0]);
  }
  /**
   * @brief Get the up vector of the Transform.
   *
   * @return The up vector.
   */
  glm::vec3 get_up_vector() const {
    glm::mat4 rotation_matrix = get_rotation_matrix();
    return glm::vec3(rotation_matrix[1]);
  }
  /**
   * @brief Function to swap the data in two Transform Components.
   * @details Swaps all of the data in the two Transforms effectively swapping
   * the actual Components.
   * @param transform A Referenced Transform to swap data with.
   */
  void swap_transform(Referenced<Transform> transform) {
    glm::mat4 temp = transform->get_transformation_matrix();
    transform->set_transformation_matrix(temp);
    transformation_matrix_ = temp;
  }
  Transform operator*(Transform transform) {
    Transform result{};
    result.set_transformation_matrix(transformation_matrix_ *
                                     transform.get_transformation_matrix());
    return result;
  }

private:
  glm::mat4 transformation_matrix_;
};

} // namespace mare

#endif