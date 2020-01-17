#include "mare/GL/GLRenderState.hpp"

// MARE
#include "mare/GL/GLBuffer.hpp"

// OpenGL
#include "GL/glew.h"

namespace mare
{
GLRenderState::~GLRenderState()
{
    glDeleteVertexArrays(1, &m_render_state_ID);
}
void GLRenderState::bind() const
{
    glBindVertexArray(m_render_state_ID);
}
void GLRenderState::unbind() const
{
    glBindVertexArray(0);
}
void GLRenderState::create()
{
    // Delete vertex array if one exists
    glDeleteVertexArrays(1, &m_render_state_ID);
    glCreateVertexArrays(1, &m_render_state_ID);
}
void GLRenderState::add_vertex_buffer(Buffer<float> *vbo)
{
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
void GLRenderState::add_vertex_buffer(Buffer<glm::vec2> *vbo)
{
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
void GLRenderState::add_vertex_buffer(Buffer<glm::vec3> *vbo)
{
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
void GLRenderState::set_index_buffer(Buffer<unsigned int> *ibo)
{
    m_is_indexed = true;
    glVertexArrayElementBuffer(m_render_state_ID, ibo->name());
    m_index_render_count = ibo->count();
}

} // namespace mare