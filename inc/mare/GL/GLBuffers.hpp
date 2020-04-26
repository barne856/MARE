#ifndef GLBUFFER
#define GLBUFFER

// MARE
#include "mare/Buffers.hpp"
#include "mare/Mare.hpp"

// Standard Library
#include <type_traits>
#include <algorithm>

// OpenGL
#include "GL/glew.h"

namespace mare
{
template <typename T>
class GLBuffer : public Buffer<T>
{
public:
    using IBuffer::buffer_ID_;
    using IBuffer::buffer_index_;
    using IBuffer::count_;
    using IBuffer::num_buffers_;
    using IBuffer::size_;
    using IBuffer::type_;
    GLBuffer(T *data, size_t size_in_bytes, BufferType buffer_type = BufferType::STATIC)
        : Buffer<T>(data, size_in_bytes, buffer_type), buffer_pointer_(nullptr), buffer_fence_(nullptr)
    {
        // Create the buffer
        glCreateBuffers(1, &buffer_ID_);
        GLbitfield flags = 0;
        switch (buffer_type)
        {
        case BufferType::STATIC:
            glNamedBufferStorage(buffer_ID_, size_in_bytes, data, flags);
            break;
        case BufferType::READ_ONLY:
            flags = GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
            break;
        case BufferType::WRITE_ONLY:
            flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
            break;
        case BufferType::READ_WRITE:
            flags = GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
            break;
        case BufferType::WRITE_ONLY_DOUBLE_BUFFERED:
            flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
            break;
        case BufferType::READ_WRITE_DOUBLE_BUFFERED:
            flags = GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
            break;
        case BufferType::WRITE_ONLY_TRIPLE_BUFFERED:
            flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
            break;
        case BufferType::READ_WRITE_TRIPLE_BUFFERED:
            flags = GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
            break;
        default:
            break;
        }
        if (buffer_type != BufferType::STATIC)
        {
            glNamedBufferStorage(buffer_ID_, num_buffers_ * size_in_bytes, nullptr, flags);
            buffer_pointer_ = static_cast<T *>(glMapNamedBufferRange(buffer_ID_, 0, num_buffers_ * size_in_bytes, flags));
            if (data)
            {
                for (int i = 0; i < num_buffers_; i++)
                {
                    flush(data, 0, size_in_bytes);
                    swap_buffer();
                }
            }
            if (num_buffers_ > 1)
            {
                buffer_fence_ = new GLsync[num_buffers_];
            }
        }
    }
    ~GLBuffer()
    {
        glDeleteBuffers(1, &this->buffer_ID_);
        for (unsigned short i = 0; i < this->num_buffers_; i++)
        {
            if (buffer_fence_)
            {
                glDeleteSync(buffer_fence_[i]);
            }
        }
        delete[] buffer_fence_;
    }
    void flush(T *data, uint32_t offset_index, size_t size_in_bytes)
    {
        assert(type_ != BufferType::STATIC);                      // Buffer must not be static
        assert(type_ != BufferType::READ_ONLY);                   // Buffer must not be read only
        assert(size_in_bytes + offset_index * sizeof(T) > size_); // buffer must have enough space to flush data

        count_ = std::max(static_cast<uint32_t>(size_in_bytes / sizeof(T)) + offset_index, count_);
        size_t write_offset = sizeof(T) * (offset_index + buffer_index_ * static_cast<uint32_t>(size_in_bytes / sizeof(T)));
        // wait for OpenGL to finish reading from the buffer
        wait_buffer();
        // write into back buffer
        std::memcpy(static_cast<void *>(&buffer_pointer_[write_offset]), data, size_in_bytes);
    }
    void clear(uint32_t offset_index)
    {
        if (!format_.stride)
        {
            count_ = offset_index;
        }
        else
        {
            count_ = static_cast<uint32_t>(sizeof(T) / format_.stride) * offset_index;
        }
    }
    T &operator[](uint32_t i)
    {
        assert(type_ != BufferType::STATIC);    // Buffer must not be static
        assert(type_ != BufferType::READ_ONLY); // Buffer must not be read only
        return buffer_pointer_[i];
    }
    T operator[](uint32_t i) const
    {
        assert(type_ != BufferType::STATIC);                     // Buffer must not be static
        assert(type_ != BufferType::WRITE_ONLY);                 // Buffer must not be write only
        assert(type_ != BufferType::WRITE_ONLY_DOUBLE_BUFFERED); // Buffer must not be write only
        assert(type_ != BufferType::WRITE_ONLY_TRIPLE_BUFFERED); // Buffer must not be write only
        T element;
        element = buffer_pointer_[i];
        return element;
    }
    void wait_buffer()
    {
        if (buffer_fence_ && (glIsSync(buffer_fence_[this->buffer_index_]) == GL_TRUE))
        {
            while (true)
            {
                GLenum result = glClientWaitSync(buffer_fence_[this->buffer_index_], GL_SYNC_FLUSH_COMMANDS_BIT, 1);
                if (result == GL_ALREADY_SIGNALED || result == GL_CONDITION_SATISFIED)
                {
                    glDeleteSync(buffer_fence_[this->buffer_index_]);
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
    void lock_buffer()
    {
        if (buffer_fence_)
        {
            if (glIsSync(buffer_fence_[this->buffer_index_]) == GL_TRUE)
            {
                glDeleteSync(buffer_fence_[this->buffer_index_]);
            }
            buffer_fence_[this->buffer_index_] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        }
    }

private:
    T *buffer_pointer_;
    GLsync *buffer_fence_;
};

class GLTexture2DBuffer : public Texture2DBuffer
{
public:
    GLTexture2DBuffer(const char *filepath);
    GLTexture2DBuffer(TextureType type, int width, int height);
    virtual ~GLTexture2DBuffer();
};

class GLFramebuffer : public Framebuffer
{
public:
    GLFramebuffer(int width, int height);
    virtual ~GLFramebuffer();
};
} // namespace mare

#endif