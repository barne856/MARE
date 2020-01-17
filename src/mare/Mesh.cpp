#include "mare/Mesh.hpp"

// MARE
#include "mare/Application.hpp"

namespace mare
{
void Mesh::render(Material *material)
{
    Application::render_mesh(this, material);
}
void Mesh::render(Material *material, glm::mat4 parent_model)
{
    Application::render_mesh(this, material, parent_model);
}
void Mesh::render(Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models)
{
    Application::render_mesh(this, material, parent_model, instance_count, models);
}
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