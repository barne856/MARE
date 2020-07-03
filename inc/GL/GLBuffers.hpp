#ifndef GLBUFFER
#define GLBUFFER

// MARE
#include "Buffers.hpp"
#include "Mare.hpp"

// Standard Library
#include <algorithm>
#include <cstring>
#include <type_traits>

// OpenGL
#include "GL/glew.h"

namespace mare {

/**
 * @brief OpenGL specific helper functions.
 */
namespace opengl {
/**
 * @brief A utility function that returns the OpenGL sized internal texture
 * format of a mare::TextureType.
 *
 * @param type The TextureType.
 * @return GLenum The OpenGL sized internal texture format
 * @see TextureType.
 */
GLenum gl_sized_tex_format(TextureType type);
/**
 * @brief A utility function that returns the OpenGL unsized internal texture
 * format of a mare::TextureType.
 *
 * @param type The TextureType
 * @return GLenum The unsized OpenGL internal texture format.
 * @see TextureType
 */
GLenum gl_tex_format(TextureType type);
/**
 * @brief A utility function that returns the size in bytes of a single pixel of
 * a TextureType.
 *
 * @param type The TextureType.
 * @return int The size in bytes of a single pixel of \p type.
 * @see TextureType
 */
int gl_tex_bytes(TextureType type);
/**
 * @brief A utility function that returns the OpenGL data type used by a
 * TextureType.
 *
 * @param type The TextureType.
 * @return GLenum The OpenGL data type.
 */
GLenum gl_tex_type(TextureType type);
} // namespace opengl

/**
 * @brief The OpenGL 4.5 implementation of the Buffer class.
 *
 * @tparam <T> The type of Buffer.
 * @see Buffer
 */
template <typename T> class GLBuffer : public Buffer<T> {
public:
  using IBuffer::buffer_ID_;
  using IBuffer::buffer_index_;
  using IBuffer::count_;
  using IBuffer::num_buffers_;
  using IBuffer::size_;
  using IBuffer::swap_buffer;
  using IBuffer::type_;
  /**
   * @brief Construct a new GLBuffer object
   *
   * @param data A pointer to the data used to initialize the GLBuffer.
   * @param size_in_bytes The size in bytes allocated for the GLBuffer.
   * @param buffer_type The BufferType.
   * @see BufferType
   * @see Buffer
   */
  GLBuffer(T *data, size_t size_in_bytes,
           BufferType buffer_type = BufferType::STATIC)
      : Buffer<T>(data, size_in_bytes, buffer_type), buffer_pointer_(nullptr),
        buffer_fence_(nullptr) {
    // Create the buffer
    glCreateBuffers(1, &buffer_ID_);
    GLbitfield flags = 0;
    switch (buffer_type) {
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
      flags = GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT |
              GL_MAP_COHERENT_BIT;
      break;
    case BufferType::WRITE_ONLY_DOUBLE_BUFFERED:
      flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
      break;
    case BufferType::READ_WRITE_DOUBLE_BUFFERED:
      flags = GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT |
              GL_MAP_COHERENT_BIT;
      break;
    case BufferType::WRITE_ONLY_TRIPLE_BUFFERED:
      flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
      break;
    case BufferType::READ_WRITE_TRIPLE_BUFFERED:
      flags = GL_MAP_WRITE_BIT | GL_MAP_READ_BIT | GL_MAP_PERSISTENT_BIT |
              GL_MAP_COHERENT_BIT;
      break;
    default:
      break;
    }
    if (buffer_type != BufferType::STATIC) {
      glNamedBufferStorage(buffer_ID_, num_buffers_ * size_in_bytes, nullptr,
                           flags);
      buffer_pointer_ = static_cast<T *>(glMapNamedBufferRange(
          buffer_ID_, 0, num_buffers_ * size_in_bytes, flags));
      if (data) {
        for (int i = 0; i < num_buffers_; i++) {
          flush(data, 0, size_in_bytes);
          swap_buffer();
        }
      }
      if (num_buffers_ > 1) {
        buffer_fence_ = new GLsync[num_buffers_];
      }
    }
  }
  /**
   * @brief Destroy the GLBuffer object.
   */
  ~GLBuffer() {
    glDeleteBuffers(1, &this->buffer_ID_);
    for (unsigned short i = 0; i < this->num_buffers_; i++) {
      if (buffer_fence_) {
        glDeleteSync(buffer_fence_[i]);
      }
    }
    delete[] buffer_fence_;
  }
  /**
   * @brief The OpenGL 4.5 implementation of the Buffer::flush(T*, uint32_t,
   * size_t) function.
   * @details The data pointer must contain allocated data at least \p
   * size_in_bytes large. The \p offset_index must not be larger than the
   * maximum amount of Buffer elements - 1. \p size_in_bytes must not be larger
   * than the space left in the Buffer after \p offset_index elements.
   *
   * @param data A pointer to the data to flush.
   * @param offset_index An index into the Buffer to start writing data.
   * @param size_in_bytes The size in bytes of the data to flush.
   */
  void flush(T *data, uint32_t offset_index, size_t size_in_bytes) {
    assert(type_ != BufferType::STATIC);    // Buffer must not be static
    assert(type_ != BufferType::READ_ONLY); // Buffer must not be read only
    assert(size_in_bytes + offset_index * sizeof(T) <=
           size_); // buffer must have enough space to flush data

    count_ = std::max(static_cast<uint32_t>(size_in_bytes / sizeof(T)) +
                          offset_index,
                      count_);
    //size_t write_offset =
    //    sizeof(T) *
    //    (offset_index +
    //     buffer_index_ * static_cast<uint32_t>(size_in_bytes / sizeof(T)));
    // wait for OpenGL to finish reading from the buffer
    wait_buffer();
    // write into back buffer
    std::memcpy(&buffer_pointer_[offset_index], data,
                size_in_bytes);
  }
  /**
   * @brief Provides write access to the buffer using the subscript operator.
   * @details BufferType must allow for writing to the Buffer.
   * @param i The index into the Buffer to write the data.
   * @return <T>& A reference into the buffer at the provided index.
   */
  T &operator[](uint32_t i) {
    assert(type_ != BufferType::STATIC);    // Buffer must not be static
    assert(type_ != BufferType::READ_ONLY); // Buffer must not be read only
    return buffer_pointer_[i];
  }
  /**
   * @brief Provides read access to the buffer using the subscript operator.
   * @details BufferType must allow for reading from the Buffer. This makes a
   * copy of the data on the client-side where the CPU can use it.
   * @param i The index into the Buffer to read data from.
   * @return <T> The data read from the buffer at the provided index.
   */
  T operator[](uint32_t i) const {
    assert(type_ != BufferType::STATIC);     // Buffer must not be static
    assert(type_ != BufferType::WRITE_ONLY); // Buffer must not be write only
    assert(type_ != BufferType::WRITE_ONLY_DOUBLE_BUFFERED); // Buffer must not
                                                             // be write only
    assert(type_ != BufferType::WRITE_ONLY_TRIPLE_BUFFERED); // Buffer must not
                                                             // be write only
    T element;
    element = buffer_pointer_[i];
    return element;
  }
  /**
   * @brief Wait for any locks to be released on the buffer before
   * proceeding. Used only on multibuffered buffers before writing data.
   */
  void wait_buffer() {
    if (buffer_fence_ &&
        (glIsSync(buffer_fence_[this->buffer_index_]) == GL_TRUE)) {
      while (true) {
        GLenum result = glClientWaitSync(buffer_fence_[this->buffer_index_],
                                         GL_SYNC_FLUSH_COMMANDS_BIT, 1);
        if (result == GL_ALREADY_SIGNALED || result == GL_CONDITION_SATISFIED) {
          glDeleteSync(buffer_fence_[this->buffer_index_]);
          return;
        } else if (result == GL_TIMEOUT_EXPIRED) {
          std::cerr << "Sync timeout expired, slow performance." << std::endl;
          continue;
        } else if (result == GL_WAIT_FAILED) {
          std::cerr << "Non-valid sync object!" << std::endl;
          return;
        }
      }
    }
  }
  /**
   * @brief Place a lock on the buffer. Any previously submitted buffer
   * operations will need to be completed before the lock is released. Used only
   * on multibuffered buffers.
   */
  void lock_buffer() {
    if (buffer_fence_) {
      if (glIsSync(buffer_fence_[this->buffer_index_]) == GL_TRUE) {
        glDeleteSync(buffer_fence_[this->buffer_index_]);
      }
      buffer_fence_[this->buffer_index_] =
          glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    }
  }
  /**
   * @brief Clear the entire Buffer to a uniform value.
   *
   * @param value The value used to clear the Buffer.
   */
  void clear(T value) {
    assert(type_ != BufferType::STATIC);    // Buffer must not be static
    assert(type_ != BufferType::READ_ONLY); // Buffer must not be read only
    for (uint32_t i = 0; i < count_; i++) {
      buffer_pointer_[i] = value;
    }
  }

private:
  T *buffer_pointer_; /**< A pointer to the data in the Buffer if Buffer is not
                         BufferType::STATIC.*/
  GLsync *buffer_fence_; /**< An OpenGL fence used to sync the Buffer.*/
};

/**
 * @brief The OpenGL 4.5 implementation of the Texture2D class
 * @see Texture2D
 */
class GLTexture2D : public Texture2D {
public:
  /**
   * @brief Construct a new GLTexture2D object from a filepath to an image.
   *
   * @param filepath The filepath to the image. Can be relative to directory
   * where the application was launched from or absolute.
   */
  GLTexture2D(const char *filepath);
  /**
   * @brief Construct a new empty GLTexture2D object.
   *
   * @param type The TextureType to create.
   * @param width The width in pixels of the Texture.
   * @param height The height in pixels of the Texture.
   * @see TextureType
   */
  GLTexture2D(TextureType type, int width, int height);
  /**
   * @brief Destroy the GLTexture2D object
   *
   */
  virtual ~GLTexture2D();
};

/**
 * @brief The OpenGL 4.5 implementation of the Framebuffer class.
 */
class GLFramebuffer : public Framebuffer {
public:
  /**
   * @brief Construct a new GLFramebuffer object
   *
   * @param width The width in pixels or fragments of the Framebuffer.
   * @param height The height in pixels or fragments of the Framebuffer.
   */
  GLFramebuffer(int width, int height);
  /**
   * @brief Destroy the GLFramebuffer object
   */
  virtual ~GLFramebuffer();
};
} // namespace mare

#endif