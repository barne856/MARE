#include "mare/Mesh.hpp"

namespace mare
{
void Mesh::translate(glm::vec3 translation)
{
    translation_ += translation;
    update_model_matrix();
}
void Mesh::set_position(glm::vec3 position)
{
    translation_ = position;
    update_model_matrix();
}
void Mesh::rotate(glm::vec3 axis, float angle)
{
    rotation_ = glm::rotate(rotation_, angle, axis);
    update_model_matrix();
}
void Mesh::set_scale(glm::vec3 scale)
{
    scale_ = scale;
    update_model_matrix();
}
} // namespace mare