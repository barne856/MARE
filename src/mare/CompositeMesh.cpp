#include "mare/CompositeMesh.hpp"
#include "mare/Application.hpp"

namespace mare
{

void CompositeMesh::render(Material *material)
{
    material->bind();
    if (Application::get_camera())
    {
        material->upload_mat4("projection", Application::get_camera()->projection());
        material->upload_mat4("view", Application::get_camera()->view());
    }
    for (Mesh *mesh : m_meshes)
    {
        mesh->render(material, transform);
    }
}

void CompositeMesh::render(Material *material, glm::mat4 parent_model)
{
    for (Mesh *mesh : m_meshes)
    {
        mesh->render(material, parent_model * transform);
    }
}

void CompositeMesh::render(Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4>* models)
{
    for (Mesh *mesh : m_meshes)
    {
        mesh->render(material, parent_model * transform, instance_count, models);
    }
}

} // namespace mare