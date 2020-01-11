#include "mare/GL/GLBuffer.hpp"
#include "GL/glew.h"

namespace mare
{
template <typename T>
void GLBuffer<T>::create(std::vector<T> &data)
{
    glDeleteBuffers(1, &m_buffer_ID);
    glCreateBuffers(1, &m_buffer_ID);
    glNamedBufferData(m_buffer_ID, sizeof(T) * data.size(), &data[0], GL_DYNAMIC_DRAW);
    m_data_size = sizeof(T) * data.size();
    if (!m_format.stride())
    {
        m_count = data.size();
    }
    else
    {
        m_count = m_data_size / m_format.stride();
    }
}

template <typename T>
void GLBuffer<T>::update(std::vector<T> &data, unsigned int offset)
{
    glNamedBufferSubData(m_buffer_ID, offset * sizeof(T), sizeof(T) * data.size(), &data[0]);
}

template <typename T>
GLBuffer<T>::~GLBuffer()
{
    glDeleteBuffers(1, &m_buffer_ID);
}

template <typename T>
unsigned int GLBuffer<T>::gl_type(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
        return GL_FLOAT;
    case ShaderDataType::Float2:
        return GL_FLOAT;
    case ShaderDataType::Float3:
        return GL_FLOAT;
    case ShaderDataType::Float4:
        return GL_FLOAT;
    case ShaderDataType::Mat3:
        return GL_FLOAT;
    case ShaderDataType::Mat4:
        return GL_FLOAT;
    case ShaderDataType::Int:
        return GL_INT;
    case ShaderDataType::Int2:
        return GL_INT;
    case ShaderDataType::Int3:
        return GL_INT;
    case ShaderDataType::Int4:
        return GL_INT;
    case ShaderDataType::Bool:
        return GL_BOOL;
    }
    return 0;
}

template class GLBuffer<float>;
template class GLBuffer<int>;
template class GLBuffer<bool>;

} // namespace mare