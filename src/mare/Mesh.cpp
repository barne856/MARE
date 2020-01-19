#include "mare/Mesh.hpp"

namespace mare
{
void Mesh::translate(glm::vec3 translation)
{
    this->translation += translation;
    update_model_matrix();
}
void Mesh::set_position(glm::vec3 position)
{
    this->translation = position;
    update_model_matrix();
}
void Mesh::rotate(glm::vec3 axis, float angle)
{
    rotation = glm::rotate(rotation, angle, axis);
    update_model_matrix();
}
void Mesh::set_scale(glm::vec3 scale)
{
    this->scale = scale;
    update_model_matrix();
}
} // namespace mare