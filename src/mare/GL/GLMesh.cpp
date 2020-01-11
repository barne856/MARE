#include "mare/GL/GLMesh.hpp"

namespace mare
{
void GLMesh::render()
{
    bind();
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