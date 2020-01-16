#include "mare/GL/GLCompositeMesh.hpp"
#include "mare/Application.hpp"

#include <iostream>

namespace mare
{

void GLCompositeMesh::render(Material *material)
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

void GLCompositeMesh::render(Material *material, glm::mat4 parent_model)
{
    for (Mesh *mesh : m_meshes)
    {
        mesh->render(material, transform * parent_model);
    }
}

void GLCompositeMesh::render(Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4>* models)
{
    for (Mesh *mesh : m_meshes)
    {
        mesh->render(material, transform * parent_model, instance_count, models);
    }
}

} // namespace mare