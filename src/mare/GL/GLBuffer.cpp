#include "mare/GL/GLBuffer.hpp"
#include "GL/glew.h"

#include <iostream>

#include "glm.hpp"

namespace mare
{
template <typename T>
void GLBuffer<T>::create(std::vector<T> &data, size_t dynamic_size_in_bytes)
{
    glDeleteBuffers(1, &m_buffer_ID);
    glCreateBuffers(1, &m_buffer_ID);
    if (dynamic_size_in_bytes)
    {
        glNamedBufferStorage(m_buffer_ID, dynamic_size_in_bytes, nullptr, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
        m_dynamic_size_in_bytes = dynamic_size_in_bytes;
        is_dynamic = true;
        if (data.size())
        {
            m_data_size = sizeof(T) * data.size();
            update(data, 0);
        }
    }
    else
    {
        glNamedBufferData(m_buffer_ID, sizeof(T) * data.size(), &data[0], GL_STATIC_DRAW);
        m_data_size = sizeof(T) * data.size();
        is_dynamic = false;
    }
    if (!m_format.stride())
    {
        m_count = unsigned int(data.size());
    }
    else
    {
        m_count = unsigned int(m_data_size / m_format.stride());
    }
}

// updates dynamic buffers with pre-allocated space. render count will increase but not decrease
template <typename T>
void GLBuffer<T>::update(std::vector<T> &data, unsigned int offset)
{
    if (is_dynamic)
    {
        if (sizeof(T) * data.size() + offset * sizeof(T) > m_dynamic_size_in_bytes)
        {
            std::cerr << "Cannot update buffer, out of range" << std::endl;
        }
        else
        {
            // increase render count, does not decrease render count if updated with zeros
            if (sizeof(T) * data.size() + offset * sizeof(T) > m_data_size)
            {
                m_data_size = sizeof(T) * data.size() + offset * sizeof(T);
                if (!m_format.stride())
                {
                    m_count = unsigned int(data.size());
                }
                else
                {
                    m_count = unsigned int(m_data_size / m_format.stride());
                }
            }
            void *p = glMapNamedBufferRange(m_buffer_ID, offset * sizeof(T), sizeof(T) * data.size(), GL_MAP_WRITE_BIT);
            memcpy(p, &data[0], sizeof(T) * data.size());
            glUnmapNamedBuffer(m_buffer_ID);
        }
    }
    else
    {
        std::cerr << "Cannot update a static buffer, set the dynamic size in bytes during buffer creation to create a dynamic buffer" << std::endl;
    }
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
template class GLBuffer<unsigned int>;
template class GLBuffer<bool>;
template class GLBuffer<glm::mat4>;
template class GLBuffer<glm::mat3>;
template class GLBuffer<glm::mat2>;
template class GLBuffer<glm::vec2>;
template class GLBuffer<glm::vec3>;
template class GLBuffer<glm::vec4>;

} // namespace mare