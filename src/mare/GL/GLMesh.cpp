#include "mare/GL/GLMesh.hpp"
#include "mare/Application.hpp"

namespace mare
{
void GLMesh::render(Material *material)
{
    bind();
    material->bind();
    if (Application::get_camera())
    {
        material->upload_mat4("projection", Application::get_camera()->projection());
        material->upload_mat4("view", Application::get_camera()->view());
    }
    material->upload_mat4("model", transform);
    if (m_vao->is_indexed())
    {
        glDrawElements(m_draw_method, GLsizei(m_vao->count()), GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawArrays(m_draw_method, 0, GLsizei(m_vao->count()));
    }
}
void GLMesh::render(Material *material, glm::mat4 model)
{
    bind();
    material->bind();
    material->upload_mat4("model", model * transform);
    if (m_vao->is_indexed())
    {
        glDrawElements(m_draw_method, GLsizei(m_vao->count()), GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        glDrawArrays(m_draw_method, 0, GLsizei(m_vao->count()));
    }
}
} // namespace mare