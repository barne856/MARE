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
        glVertexArrayAttribFormat(m_render_state_ID, m_attribute_index, static_cast<GLuint>(element.component_count()), gl_type(element.data_type), element.normalized ? GL_TRUE : GL_FALSE, static_cast<GLint>(element.offset));
        glVertexArrayAttribBinding(m_render_state_ID, m_attribute_index, static_cast<GLuint>(m_vertex_buffer_count));
        glVertexArrayVertexBuffer(m_render_state_ID, static_cast<GLuint>(m_vertex_buffer_count), static_cast<GLuint>(vbo->name()), 0, static_cast<GLsizei>(vbo->format().stride));
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

GLenum GLRenderState::gl_type(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::FLOAT:
        return GL_FLOAT;
    case ShaderDataType::VEC2:
        return GL_FLOAT;
    case ShaderDataType::VEC3:
        return GL_FLOAT;
    case ShaderDataType::VEC4:
        return GL_FLOAT;
    case ShaderDataType::MAT2:
        return GL_FLOAT;
    case ShaderDataType::MAT3:
        return GL_FLOAT;
    case ShaderDataType::MAT4:
        return GL_FLOAT;
    case ShaderDataType::INT:
        return GL_INT;
    case ShaderDataType::INT2:
        return GL_INT;
    case ShaderDataType::INT3:
        return GL_INT;
    case ShaderDataType::INT4:
        return GL_INT;
    case ShaderDataType::BOOL:
        return GL_BOOL;
    case ShaderDataType::BYTE:
        return GL_UNSIGNED_BYTE;
    case ShaderDataType::SHORT:
        return GL_SHORT;
    case ShaderDataType::UNSIGNED_SHORT:
        return GL_UNSIGNED_SHORT;
    case ShaderDataType::UNSIGNED_INT:
        return GL_UNSIGNED_INT;
    default:
        return GL_NONE;
    }
    return 0;
}

} // namespace mare