// MARE
#include "mare/GL/GLRenderState.hpp"

// OpenGL
#include "GL/glew.h"

namespace mare
{
GLRenderState::GLRenderState()
{
    glCreateVertexArrays(1, &m_render_state_ID);
}
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

void GLRenderState::set_vertex_buffer(Buffer<float> *vbo)
{
    vertex_buffer = vbo;
    for (const auto &element : vbo->format())
    {
        glEnableVertexArrayAttrib(m_render_state_ID, m_attribute_index);
        glVertexArrayAttribFormat(m_render_state_ID, m_attribute_index, element.component_count(), gl_type(element.data_type), element.normalized ? GL_TRUE : GL_FALSE, element.offset);
        glVertexArrayAttribBinding(m_render_state_ID, m_attribute_index, m_vertex_buffer_count);
        glVertexArrayVertexBuffer(m_render_state_ID, m_vertex_buffer_count, vbo->name(), 0, vbo->format().stride);
        m_attribute_index++;
    }
    m_vertex_buffer_count++;
    m_vertex_render_count = vbo->count();
}
void GLRenderState::set_index_buffer(Buffer<unsigned int> *ibo)
{
    index_buffer = ibo;
    glVertexArrayElementBuffer(m_render_state_ID, ibo->name());
    m_index_render_count = ibo->count();
}

GLenum GLRenderState::gl_type(LinalgDataType type)
{
    switch (type)
    {
    case LinalgDataType::FLOAT:
        return GL_FLOAT;
    case LinalgDataType::VEC2:
        return GL_FLOAT;
    case LinalgDataType::VEC3:
        return GL_FLOAT;
    case LinalgDataType::VEC4:
        return GL_FLOAT;
    case LinalgDataType::MAT2:
        return GL_FLOAT;
    case LinalgDataType::MAT3:
        return GL_FLOAT;
    case LinalgDataType::MAT4:
        return GL_FLOAT;
    case LinalgDataType::INT:
        return GL_INT;
    case LinalgDataType::INT2:
        return GL_INT;
    case LinalgDataType::INT3:
        return GL_INT;
    case LinalgDataType::INT4:
        return GL_INT;
    case LinalgDataType::BOOL:
        return GL_BOOL;
    case LinalgDataType::BYTE:
        return GL_UNSIGNED_BYTE;
    case LinalgDataType::SHORT:
        return GL_SHORT;
    case LinalgDataType::UNSIGNED_SHORT:
        return GL_UNSIGNED_SHORT;
    case LinalgDataType::UNSIGNED_INT:
        return GL_UNSIGNED_INT;
    default:
        return GL_NONE;
    }
    return 0;
}

} // namespace mare