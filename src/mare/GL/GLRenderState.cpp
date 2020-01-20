#include "mare/GL/GLRenderState.hpp"

// MARE
#include "mare/GL/GLBuffer.hpp"

// OpenGL
#include "GL/glew.h"

namespace mare
{
template <typename T>
GLRenderState<T>::~GLRenderState()
{
    glDeleteVertexArrays(1, &m_render_state_ID);
}
template <typename T>
void GLRenderState<T>::bind() const
{
    glBindVertexArray(m_render_state_ID);
}
template <typename T>
void GLRenderState<T>::unbind() const
{
    glBindVertexArray(0);
}
template <typename T>
void GLRenderState<T>::create()
{
    // Delete vertex array if one exists
    glDeleteVertexArrays(1, &m_render_state_ID);
    glCreateVertexArrays(1, &m_render_state_ID);
}
template <typename T>
void GLRenderState<T>::add_vertex_buffer(Buffer<T> *vbo)
{
    vertex_buffers.push_back(vbo);
    for (const auto &element : vbo->format())
    {
        glEnableVertexArrayAttrib(m_render_state_ID, m_attribute_index);
        glVertexArrayAttribFormat(m_render_state_ID, m_attribute_index, element.component_count(), GLBuffer<float>::gl_type(element.type()), element.m_normalized ? GL_TRUE : GL_FALSE, element.m_offset);
        glVertexArrayAttribBinding(m_render_state_ID, m_attribute_index, m_vertex_buffer_count);
        glVertexArrayVertexBuffer(m_render_state_ID, m_vertex_buffer_count, vbo->name(), 0, vbo->format().stride());
        m_attribute_index++;
    }
    m_vertex_buffer_count++;
    m_vertex_render_count = vbo->count();
}
template <typename T>
void GLRenderState<T>::set_index_buffer(Buffer<unsigned int> *ibo)
{
    index_buffer = ibo;
    m_is_indexed = true;
    glVertexArrayElementBuffer(m_render_state_ID, ibo->name());
    m_index_render_count = ibo->count();
}

template class GLRenderState<float>;
template class GLRenderState<glm::vec2>;
template class GLRenderState<glm::vec3>;
template class GLRenderState<glm::vec4>;

} // namespace mare