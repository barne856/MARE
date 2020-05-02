#include "mare/Transform.hpp"

namespace mare
{
void Transform::translate(glm::vec3 translation)
{
    translation_ += translation;
    update_model_matrix();
}
void Transform::set_position(glm::vec3 position)
{
    translation_ = position;
    update_model_matrix();
}
void Transform::rotate(glm::vec3 axis, float angle)
{
    rotation_ = glm::rotate(rotation_, angle, axis);
    update_model_matrix();
}
void Transform::set_rotation(glm::vec3 axis, float angle)
{
    rotation_ = glm::rotate(glm::mat4(1.0f), angle, axis);
    update_model_matrix();
}
void Transform::set_scale(glm::vec3 scale)
{
    scale_ = scale;
    update_model_matrix();
}
void Transform::set_up_vector(glm::vec3 up)
{
    up_vector_ = up;
}
void Transform::set_forward_vector(glm::vec3 forward)
{
    forward_vector_ = forward;
}
glm::mat4 Transform::get_view() const
{
    return glm::lookAt(translation_, translation_ + glm::mat3(rotation_) * forward_vector_, up_vector_);
}
void Transform::face_towards(glm::vec3 direction)
{
    direction = glm::normalize(direction);
    glm::vec3 axis = glm::cross(forward_vector_, direction);
    float angle = acosf(glm::dot(forward_vector_, direction) / (glm::length(forward_vector_) * glm::length(direction)));
    set_rotation(axis, angle);
}
void Transform::look_at(glm::vec3 center)
{
    face_towards(center - translation_);
}
glm::vec3 Transform::get_direction() const
{
    return glm::mat3(rotation_) * forward_vector_;
}
glm::vec3 Transform::get_up_vector() const
{
    return up_vector_;
}
void Transform::update_model_matrix()
{
    transform_ = glm::translate(glm::mat4(1.0f), translation_) * rotation_ * glm::scale(glm::mat4(1.0f), scale_);
    normal_ = glm::transpose(glm::inverse(glm::mat3(rotation_ * glm::scale(glm::mat4(1.0f), scale_))));
}
} // namespace mare