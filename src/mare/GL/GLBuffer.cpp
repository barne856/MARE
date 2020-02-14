// MARE
#include "mare/GL/GLBuffer.hpp"

// Standard Library
#include <iostream>
#include <algorithm>

// External Libraries
#include "glm.hpp"

namespace mare
{
template <typename T>
GLBuffer<T>::GLBuffer(std::vector<T> *data, BufferType buffer_type, size_t size_in_bytes)
    : Buffer(data, buffer_type, size_in_bytes), buffer_pointer(nullptr), buffer_fence(nullptr)
{
    // Create the buffer
    glCreateBuffers(1, &m_buffer_ID);
    GLbitfield flags = 0;
    switch (buffer_type)
    {
    case BufferType::STATIC:
        glNamedBufferStorage(m_buffer_ID, m_data_size, &(*data)[0], 0);
        buffer_pointer = nullptr;
        break;
    case BufferType::READ_ONLY:
        flags = GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(m_buffer_ID, m_data_size, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(m_buffer_ID, 0, m_data_size, flags));
        if (data)
        {
            flush(*data, 0);
        }
        break;
    case BufferType::WRITE_ONLY:
        flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(m_buffer_ID, m_data_size, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(m_buffer_ID, 0, m_data_size, flags));
        if (data)
        {
            flush(*data, 0);
        }
        break;
    case BufferType::READ_WRITE:
        flags = GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(m_buffer_ID, m_data_size, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(m_buffer_ID, 0, m_data_size, flags));
        if (data)
        {
            flush(*data, 0);
        }
        break;
    case BufferType::READ_ONLY_DOUBLE_BUFFERED:
        flags = GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(m_buffer_ID, m_num_buffers * m_data_size, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(m_buffer_ID, 0, m_num_buffers * m_data_size, flags));
        if (data)
        {
            flush(*data, 0);
            flush(*data, data->size());
        }
        buffer_fence = new GLsync[m_num_buffers];
        break;
    case BufferType::WRITE_ONLY_DOUBLE_BUFFERED:
        flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(m_buffer_ID, m_num_buffers * m_data_size, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(m_buffer_ID, 0, m_num_buffers * m_data_size, flags));
        if (data)
        {
            flush(*data, 0);
            flush(*data, data->size());
        }
        buffer_fence = new GLsync[m_num_buffers];
        break;
    case BufferType::READ_WRITE_DOUBLE_BUFFERED:
        flags = GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(m_buffer_ID, m_num_buffers * m_data_size, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(m_buffer_ID, 0, m_num_buffers * m_data_size, flags));
        if (data)
        {
            flush(*data, 0);
            flush(*data, data->size());
        }
        buffer_fence = new GLsync[m_num_buffers];
        break;
    case BufferType::READ_ONLY_TRIPLE_BUFFERED:
        flags = GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(m_buffer_ID, m_num_buffers * m_data_size, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(m_buffer_ID, 0, m_num_buffers * m_data_size, flags));
        if (data)
        {
            flush(*data, 0);
            flush(*data, data->size());
            flush(*data, 2 * data->size());
        }
        buffer_fence = new GLsync[m_num_buffers];
        break;
    case BufferType::WRITE_ONLY_TRIPLE_BUFFERED:
        flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(m_buffer_ID, m_num_buffers * m_data_size, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(m_buffer_ID, 0, m_num_buffers * m_data_size, flags));
        if (data)
        {
            flush(*data, 0);
            flush(*data, data->size());
            flush(*data, 2 * data->size());
        }
        buffer_fence = new GLsync[m_num_buffers];
        break;
    case BufferType::READ_WRITE_TRIPLE_BUFFERED:
        flags = GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(m_buffer_ID, m_num_buffers * m_data_size, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(m_buffer_ID, 0, m_num_buffers * m_data_size, flags));
        if (data)
        {
            flush(*data, 0);
            flush(*data, data->size());
            flush(*data, 2 * data->size());
        }
        buffer_fence = new GLsync[m_num_buffers];
        break;
    }
}

template <typename T>
GLBuffer<T>::~GLBuffer()
{
    glDeleteBuffers(1, &m_buffer_ID);
    for (unsigned short i = 0; i < m_num_buffers; i++)
    {
        glDeleteSync(buffer_fence[i]);
    }
    delete[] buffer_fence;
}

template <typename T>
void GLBuffer<T>::flush(std::vector<T> &data, unsigned int offset)
{
    if (m_buffer_type != BufferType::STATIC)
    {
        if (sizeof(T) * data.size() + offset * sizeof(T) > m_data_size)
        {
            std::cerr << "Cannot update buffer, out of range" << std::endl;
        }
        else
        {
            m_count = std::max(data.size() + offset, m_count);
            unsigned int write_offset = offset;
            if (m_num_buffers)
            {
                write_offset = offset + ((m_buffer_index + 1) % m_num_buffers) * (m_data_size / sizeof(T));
            }
            // write into back buffer
            memcpy(static_cast<void *>(&buffer_pointer[write_offset]), &data[0], data.size() * sizeof(T));
        }
    }
    else
    {
        std::cerr << "Cannot update a static buffer, set the dynamic size in bytes during buffer creation to create a dynamic buffer" << std::endl;
    }
}

template <typename T>
void GLBuffer<T>::clear(unsigned int offset)
{
    if (!m_format.stride)
    {
        m_count = sizeof(T) * offset / sizeof(T);
    }
    else
    {
        m_count = sizeof(T) * offset / m_format.stride;
    }
}

template <typename T>
T &GLBuffer<T>::operator[](unsigned int i)
{
    // if buffer can be written to by the CPU
    //if (m_buffer_type > 3)
    //{
    return buffer_pointer[i];
    //}
    //else
    //{
    //    std::cerr << "Don't write to this buffer!" << std::endl;
    //}
    //return;
}

template <typename T>
T GLBuffer<T>::operator[](unsigned int i) const
{
    // if buffer can be read by the CPU
    //if (m_buffer_type < 7)
    //{
    T element;
    element = buffer_pointer[i];
    return element;
    //}
    //else
    //{
    //    std::cerr << "Don't read from this buffer!" << std::endl;
    //}
    //return;
}

template <typename T>
void GLBuffer<T>::wait_buffer()
{
    if (buffer_fence && (glIsSync(buffer_fence[m_buffer_index]) == GL_TRUE))
    {
        while (true)
        {
            GLenum result = glClientWaitSync(buffer_fence[m_buffer_index], GL_SYNC_FLUSH_COMMANDS_BIT, 1);
            if (result == GL_ALREADY_SIGNALED || result == GL_CONDITION_SATISFIED)
            {
                glDeleteSync(buffer_fence[m_buffer_index]);
                return;
            }
            else if (result == GL_TIMEOUT_EXPIRED)
            {
                std::cerr << "Sync timeout expired, slow performance." << std::endl;
                continue;
            }
            else if (result == GL_WAIT_FAILED)
            {
                std::cerr << "Non-valid sync object!" << std::endl;
                return;
            }
        }
    }
}
template <typename T>
void GLBuffer<T>::lock_buffer()
{
    if (buffer_fence)
    {
        if (glIsSync(buffer_fence[m_buffer_index]) == GL_TRUE)
        {
            glDeleteSync(buffer_fence[m_buffer_index]);
        }
        buffer_fence[m_buffer_index] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    }
}

template class GLBuffer<float>;
template class GLBuffer<int>;
template class GLBuffer<unsigned int>;
template class GLBuffer<bool>;
template class GLBuffer<glm::vec2>;
template class GLBuffer<glm::vec3>;
template class GLBuffer<glm::vec4>;
template class GLBuffer<glm::mat2>;
template class GLBuffer<glm::mat3>;
template class GLBuffer<glm::mat4>;

} // namespace mare