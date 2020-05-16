#ifndef TRANSFORM
#define TRANSFORM

// MARE
#include "mare/Mare.hpp"

// External Libraries
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"

namespace mare {
class Transform : virtual public Component {
public:
  Transform()
      : translation_(glm::vec3(0.0f)), rotation_(glm::mat4(1.0f)),
        scale_(glm::vec3(1.0f)), transform_(glm::mat4(1.0f)),
        normal_(glm::mat3(1.0f)), up_vector_(glm::vec3(0.0f, 1.0f, 0.0f)),
        forward_vector_(glm::vec3(0.0f, 0.0f, -1.0f)) {}
  virtual ~Transform() {}
  void translate(glm::vec3 translation) {
    translation_ += translation;
    update_model_matrix();
  }
  void set_position(glm::vec3 position) {
    translation_ = position;
    update_model_matrix();
  }
  void rotate(glm::vec3 axis, float angle) {
    rotation_ = glm::rotate(rotation_, angle, axis);
    update_model_matrix();
  }
  void set_rotation(glm::vec3 axis, float angle) {
    rotation_ = glm::rotate(glm::mat4(1.0f), angle, axis);
    update_model_matrix();
  }
  void set_scale(glm::vec3 scale) {
    scale_ = scale;
    update_model_matrix();
  }
  void set_up_vector(glm::vec3 up) { up_vector_ = up; }
  void set_forward_vector(glm::vec3 forward) { forward_vector_ = forward; }
  void face_towards(glm::vec3 direction) {
    direction = glm::normalize(direction);
    glm::vec3 axis = glm::cross(forward_vector_, direction);
    float angle =
        acosf(glm::dot(forward_vector_, direction) /
              (glm::length(forward_vector_) * glm::length(direction)));
    set_rotation(axis, angle);
  }
  void look_at(glm::vec3 center) { face_towards(center - translation_); }
  glm::mat4 get_view() const {
    return glm::lookAt(translation_,
                       translation_ + glm::mat3(rotation_) * forward_vector_,
                       up_vector_);
  }
  glm::vec3 get_direction() const {
    return glm::mat3(rotation_) * forward_vector_;
  }
  glm::vec3 get_up_vector() const { return up_vector_; }
  inline glm::vec3 get_position() const { return translation_; }
  inline glm::vec3 get_scale() const { return scale_; }
  inline glm::mat4 get_model() const { return transform_; }
  inline glm::mat3 get_normal() const { return normal_; }

protected:
  void update_model_matrix() {
    transform_ = glm::translate(glm::mat4(1.0f), translation_) * rotation_ *
                 glm::scale(glm::mat4(1.0f), scale_);
    normal_ = glm::transpose(glm::inverse(
        glm::mat3(rotation_ * glm::scale(glm::mat4(1.0f), scale_))));
  }

  glm::vec3 translation_;
  glm::mat4 rotation_;
  glm::vec3 scale_;
  glm::mat4 transform_;
  glm::mat3 normal_;
  glm::vec3 up_vector_;
  glm::vec3 forward_vector_;
};

} // namespace mare

#endif