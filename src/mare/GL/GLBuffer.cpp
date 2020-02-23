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
    glCreateBuffers(1, &buffer_ID_);
    GLbitfield flags = 0;
    switch (buffer_type)
    {
    case BufferType::STATIC:
        glNamedBufferStorage(buffer_ID_, data_size_, &(*data)[0], 0);
        buffer_pointer = nullptr;
        break;
    case BufferType::READ_ONLY:
        flags = GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(buffer_ID_, data_size_, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(buffer_ID_, 0, data_size_, flags));
        if (data)
        {
            flush(*data, 0);
        }
        break;
    case BufferType::WRITE_ONLY:
        flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(buffer_ID_, data_size_, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(buffer_ID_, 0, data_size_, flags));
        if (data)
        {
            flush(*data, 0);
        }
        break;
    case BufferType::READ_WRITE:
        flags = GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(buffer_ID_, data_size_, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(buffer_ID_, 0, data_size_, flags));
        if (data)
        {
            flush(*data, 0);
        }
        break;
    case BufferType::READ_ONLY_DOUBLE_BUFFERED:
        flags = GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(buffer_ID_, num_buffers_ * data_size_, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(buffer_ID_, 0, num_buffers_ * data_size_, flags));
        if (data)
        {
            flush(*data, 0);
            flush(*data, data->size());
        }
        buffer_fence = new GLsync[num_buffers_];
        break;
    case BufferType::WRITE_ONLY_DOUBLE_BUFFERED:
        flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(buffer_ID_, num_buffers_ * data_size_, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(buffer_ID_, 0, num_buffers_ * data_size_, flags));
        if (data)
        {
            flush(*data, 0);
            flush(*data, data->size());
        }
        buffer_fence = new GLsync[num_buffers_];
        break;
    case BufferType::READ_WRITE_DOUBLE_BUFFERED:
        flags = GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(buffer_ID_, num_buffers_ * data_size_, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(buffer_ID_, 0, num_buffers_ * data_size_, flags));
        if (data)
        {
            flush(*data, 0);
            flush(*data, data->size());
        }
        buffer_fence = new GLsync[num_buffers_];
        break;
    case BufferType::READ_ONLY_TRIPLE_BUFFERED:
        flags = GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(buffer_ID_, num_buffers_ * data_size_, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(buffer_ID_, 0, num_buffers_ * data_size_, flags));
        if (data)
        {
            flush(*data, 0);
            flush(*data, data->size());
            flush(*data, 2 * data->size());
        }
        buffer_fence = new GLsync[num_buffers_];
        break;
    case BufferType::WRITE_ONLY_TRIPLE_BUFFERED:
        flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(buffer_ID_, num_buffers_ * data_size_, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(buffer_ID_, 0, num_buffers_ * data_size_, flags));
        if (data)
        {
            flush(*data, 0);
            flush(*data, data->size());
            flush(*data, 2 * data->size());
        }
        buffer_fence = new GLsync[num_buffers_];
        break;
    case BufferType::READ_WRITE_TRIPLE_BUFFERED:
        flags = GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
        glNamedBufferStorage(buffer_ID_, num_buffers_ * data_size_, nullptr, flags);
        buffer_pointer = static_cast<T *>(glMapNamedBufferRange(buffer_ID_, 0, num_buffers_ * data_size_, flags));
        if (data)
        {
            flush(*data, 0);
            flush(*data, data->size());
            flush(*data, 2 * data->size());
        }
        buffer_fence = new GLsync[num_buffers_];
        break;
    }
}

template <typename T>
GLBuffer<T>::~GLBuffer()
{
    glDeleteBuffers(1, &buffer_ID_);
    for (unsigned short i = 0; i < num_buffers_; i++)
    {
        glDeleteSync(buffer_fence[i]);
    }
    delete[] buffer_fence;
}

template <typename T>
void GLBuffer<T>::flush(std::vector<T> &data, size_t offset)
{
    if (buffer_type_ != BufferType::STATIC)
    {
        if (sizeof(T) * data.size() + offset * sizeof(T) > data_size_)
        {
            std::cerr << "Cannot update buffer, out of range" << std::endl;
        }
        else
        {
            count_ = std::max(data.size() + offset, count_);
            size_t write_offset = offset;
            if (num_buffers_)
            {
                write_offset = offset + ((buffer_index_ + 1) % num_buffers_) * (data_size_ / sizeof(T));
            }
            // wait for OpenGL to finish reading from the buffer
            wait_buffer();
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
    if (!format_.stride)
    {
        count_ = sizeof(T) * offset / sizeof(T);
    }
    else
    {
        count_ = sizeof(T) * offset / format_.stride;
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
    if (buffer_fence && (glIsSync(buffer_fence[buffer_index_]) == GL_TRUE))
    {
        while (true)
        {
            GLenum result = glClientWaitSync(buffer_fence[buffer_index_], GL_SYNC_FLUSH_COMMANDS_BIT, 1);
            if (result == GL_ALREADY_SIGNALED || result == GL_CONDITION_SATISFIED)
            {
                glDeleteSync(buffer_fence[buffer_index_]);
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
        if (glIsSync(buffer_fence[buffer_index_]) == GL_TRUE)
        {
            glDeleteSync(buffer_fence[buffer_index_]);
        }
        buffer_fence[buffer_index_] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
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