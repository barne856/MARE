// MARE
#include "mare/GL/GLRenderState.hpp"

// OpenGL
#include "GL/glew.h"

namespace mare
{
GLRenderState::GLRenderState()
{
    glCreateVertexArrays(1, &render_state_ID_);
}
GLRenderState::~GLRenderState()
{
    glDeleteVertexArrays(1, &render_state_ID_);
}
void GLRenderState::bind() const
{
    glBindVertexArray(render_state_ID_);
}

void GLRenderState::unbind() const
{
    glBindVertexArray(0);
}

void GLRenderState::set_vertex_buffer(Scoped<Buffer<float>> vbo)
{
    vertex_buffer_ = std::move(vbo);
    for (const auto &element : vertex_buffer_->format())
    {
        glEnableVertexArrayAttrib(render_state_ID_, attribute_index_);
        glVertexArrayAttribFormat(render_state_ID_, attribute_index_, static_cast<GLuint>(element.component_count()), gl_type(element.data_type), element.normalized ? GL_TRUE : GL_FALSE, static_cast<GLint>(element.offset));
        glVertexArrayAttribBinding(render_state_ID_, attribute_index_, static_cast<GLuint>(vertex_buffer_count_));
        glVertexArrayVertexBuffer(render_state_ID_, static_cast<GLuint>(vertex_buffer_count_), static_cast<GLuint>(vertex_buffer_->name()), 0, static_cast<GLsizei>(vertex_buffer_->format().stride));
        attribute_index_++;
    }
    vertex_buffer_count_++;
    vertex_render_count_ = vertex_buffer_->count();
}
void GLRenderState::set_index_buffer(Scoped<Buffer<unsigned int>> ibo)
{
    index_buffer_ = std::move(ibo);
    glVertexArrayElementBuffer(render_state_ID_, index_buffer_->name());
    index_render_count_ = index_buffer_->count();
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