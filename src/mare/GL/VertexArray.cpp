// MARE
#include "mare/GL/VertexArray.hpp"

// OpenGL
#include "GL/glew.h"

namespace mare
{
VertexArray::VertexArray()
    : m_vertex_array_ID(0), m_buffer_count(0), m_attribute_index(0), m_render_count(0), m_indexed_render_count(0), m_is_indexed(false) {}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_vertex_array_ID);
}

void VertexArray::bind() const
{
    glBindVertexArray(m_vertex_array_ID);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::create()
{
    glDeleteVertexArrays(1, &m_vertex_array_ID);
    glCreateVertexArrays(1, &m_vertex_array_ID);
}

void VertexArray::add_vertex_buffer(GLBuffer<float> *vbo)
{
    for (const auto &element : vbo->format())
    {
        glEnableVertexArrayAttrib(m_vertex_array_ID, m_attribute_index);
        glVertexArrayAttribFormat(m_vertex_array_ID, m_attribute_index, element.component_count(), GLBuffer<float>::gl_type(element.type()), element.m_normalized ? GL_TRUE : GL_FALSE, element.m_offset);
        glVertexArrayAttribBinding(m_vertex_array_ID, m_attribute_index, m_buffer_count);
        glVertexArrayVertexBuffer(m_vertex_array_ID, m_buffer_count, vbo->name(), 0, vbo->format().stride());
        m_attribute_index++;
    }
    m_buffer_count++;
    m_render_count = vbo->count();
}

void VertexArray::set_index_buffer(GLBuffer<GLuint> *ibo)
{
    m_is_indexed = true;
    glVertexArrayElementBuffer(m_vertex_array_ID, ibo->name());
    m_indexed_render_count = ibo->count();
}

size_t VertexArray::count() const
{
    if (m_is_indexed)
    {
        return m_indexed_render_count;
    }
    else
    {
        return m_render_count;
    }
}

bool VertexArray::is_indexed() const
{
    return m_is_indexed;
}
} // namespace mare