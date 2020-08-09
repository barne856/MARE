#ifndef BUFFERS
#define BUFFERS

// MARE
#include "Mare.hpp"
// Standard Library
#include <iostream>
#include <string>
#include <vector>

namespace mare {
/**
 * @brief Defines the access and storage properties of a Buffer.
 * @see Buffer.
 */
enum class BufferType {
  STATIC = 0, /**< The buffer is written to once upon creation and the data
                 cannot be changed by any client-side (CPU) operation. Data must
                 be supplied at the time of creation. This is the default
                 BufferType.*/
  READ_ONLY,  /**< The buffer can be read from the client (CPU) but not written
                 to. Data must be supplied at the time of creation.*/
  WRITE_ONLY, /**< The buffer can be written to by the client (CPU) but not read
                 from.*/
  READ_WRITE, /**< The buffer can be read from and written to by the client
                 (CPU).*/
  WRITE_ONLY_DOUBLE_BUFFERED, /**< Same as WRITE_ONLY but a back buffer is also
                                 created which can be swapped with the front
                                 buffer using IBuffer::swap_buffer().*/
  WRITE_ONLY_TRIPLE_BUFFERED, /**< Same as WRITE_ONLY but two back buffers are
                                 also created which can be cycled though with
                                 IBuffer::swap_buffer().*/
  READ_WRITE_DOUBLE_BUFFERED, /**< Same as READ_WRITE but a back buffer is also
                                 created which can be swapped with the front
                                 buffer using IBuffer::swap_buffer().*/
  READ_WRITE_TRIPLE_BUFFERED  /**< Same as READ_WRITE but two back buffers are
                                   also created which can be cycled though with
                                   IBuffer::swap_buffer().*/
};
/**
 * @brief The type of attribute used in a BufferAttribute to describe the data
 * stored in a Buffer.
 * @details An AttributeType is stored in a BufferAttribute along with the name
 * of the attribute that will be used to identify the resource in the
 * glsl shader.
 * @see BufferAttribute
 */
enum class AttributeType {
  POSITION_2D, /**< Represents 2D vertex data and is the equivalent of the glsl
                  input attribute `vec2`. Data in buffer will be interpreted as
                  two sequential floating point numbers (x,y).*/
  POSITION_3D,  /**< Represents 3D vertex data and is the equivalent of the glsl
                    input attribute `vec3`. Data in buffer will be interpreted as
                    three sequential floating point numbers (x,y,z).*/
  NORMAL,      /**< Represents 3D normal data and is the equivalent of the glsl
                        input attribute `vec3`. Data in buffer will be interpreted as
                        three sequential floating point numbers (x,y,z).*/
  COLOR, /**< Represents RGBA color data and is the equivalent of the glsl input
            attribute `vec4`. Data in the buffer will be interpreted as four
            sequatial floating point numbers (r,g,b,a).*/
  TEXTURE_MAP, /**< Represents 2D texture coordinate data and is the equivalent
                  of the glsl input attribute `vec2`. Data in buffer will be
                  interpreted as two sequential floating point numbers
                  (u,v).*/
  UNIFORM,     /**< Represents arbitrary data and is the equivalent of the glsl
                  uniform block using the std140 layout. Data will be
                  interpreted as specified by the uniform block definition in the
                  shader.*/
  STORAGE      /**< Represetns arbitrary data and is the equivalent of the glsl
                  storage block (uniform buffer) using the std430 layout. Data will
                  be interpreted as specified by the storage block in the shader.*/
};
/**
 * @brief An element of a BufferFormat.
 * @see BufferFormat
 */
class BufferAttibrute {
public:
  /**
   * @brief Construct a new BufferAttibrute object.
   *
   * @param attrib_type The AttributeType describing the data in the buffer.
   * This must match the type of data in the shader resource with the same name
   * as \p attrib_name.
   * @param attrib_name The name of the attribute used link the data in the
   * buffer to a resource in the shader. This must match the name of the
   * resourse in the shader exactly.
   */
  BufferAttibrute(AttributeType attrib_type, std::string attrib_name)
      : type(attrib_type), name(attrib_name), offset(0), size(0) {}
  ~BufferAttibrute(){};
  /**
   * @brief Returns the number of components (or dimensions) in an attribute.
   * @details The number of components is returned according to the following
   * table:
   * AttributeType  | component_count()
   * -------------- | ---------------
   * POSITION_2D     | 2
   * POSITION_3D     | 3
   * NORMAL         | 3
   * COLOR          | 4
   * TEXTURE_MAP    | 2
   * UNIFORM        | 1
   * STORAGE        | 1
   *
   * @return The number of components in the attribute.
   */
  const uint32_t component_count() const {
    switch (type) {
    case AttributeType::POSITION_2D:
      return 2;
      break;
    case AttributeType::POSITION_3D:
      return 3;
      break;
    case AttributeType::NORMAL:
      return 3;
      break;
    case AttributeType::COLOR:
      return 4;
      break;
    case AttributeType::TEXTURE_MAP:
      return 2;
      break;
    default:
      return 1;
      break;
    }
  }
  AttributeType
      type;         /**< The AttributeType describing the data in the buffer.*/
  std::string name; /**< The name of the attribute used for uploading to a
                       shader resource.*/
  size_t offset;    /**< The offset in bytes into the buffer where the first
                       element of this data begins.*/
  size_t size;      /**< The size in bytes of a single element of this data.*/
};

// A BufferFormat, when applied to a Buffer will tell the shader how the
// information is stored in the buffer, i.e. interlaced or stacked, offsets,
// etc. Each Buffer can have a Format with one or more elements or muliple
// Buffers can be used each with their own BufferFormat

/**
 * @brief A format that can be applied to a Buffer to describe the layout of
 * the data it contains.
 * @details A BufferFormat, when applied to a Buffer with
 * Buffer::set_format(BufferFormat) will specify how the data is arranged in
 * that buffer. The BufferFormat contains a `std::vector` of BufferAttributes
 * which are interpreted by the buffer as being interleaved in the buffer. For
 * example a BufferFormat with POSITION_3D, NORMAL, and TEXTURE_MAP attributes
 * in that order will be interpreted as having some number of vertex data
 * packets that consist of 8 floats where the first three correspond to the
 * POSITION_3D attribute, the next three correspond to the NORMAL attribute, and
 * the last two correspond to the TEXTURE_MAP attribute.
 */
class BufferFormat {
public:
  /**
   * @brief Construct a new empty BufferFormat object.
   */
  BufferFormat() : stride(0), attributes_({}) {}
  /**
   * @brief Construct a new BufferFormat object that contains the supplied
   * attributes.
   *
   * @param attributes The attributes supplied to the BufferFormat. Can be
   * supplied using an initializer list.
   */
  BufferFormat(const std::initializer_list<BufferAttibrute> &attributes)
      : stride(0), attributes_(attributes) {}
  /**
   * @brief Obtain a reference to the vector of BufferAttribute elements
   * contained in the BufferFormat.
   *
   * @return A reference to the vector of BufferAttribute elements contained in
   * the BufferFormat.
   */
  std::vector<BufferAttibrute> &attributes() { return attributes_; }
  /**
   * @brief Obtain an iterator pointing to the begining of the BufferAttributes.
   *
   * @return An iterator pointing to the begining of the BufferAttributes.
   */
  std::vector<BufferAttibrute>::iterator begin() { return attributes_.begin(); }
  /**
   * @brief Obtain an iterator pointing to the end of the BufferAttributes.
   *
   * @return An iterator pointing to the end of the BufferAttributes.
   */
  std::vector<BufferAttibrute>::iterator end() { return attributes_.end(); }
  /**
   * @brief Obtain a const iterator pointing to the begining of the
   * BufferAttributes.
   *
   * @return A const iterator pointing to the begining of the BufferAttributes.
   */
  std::vector<BufferAttibrute>::const_iterator begin() const {
    return attributes_.begin();
  }
  /**
   * @brief Obtain a const iterator pointing to the end of the BufferAttributes.
   *
   * @return A const iterator pointing to the end of the BufferAttributes.
   */
  std::vector<BufferAttibrute>::const_iterator end() const {
    return attributes_.end();
  }
  size_t stride; /**< The stride in bytes bewteen attributes of the same type in
                    the buffer. The total size of all the BufferAttributes.*/

private:
  std::vector<BufferAttibrute> attributes_;
};

/**
 * @brief An abstract base class for Buffers handled by the Rendering API.
 */
class IBuffer {
public:
  IBuffer() {}
  virtual ~IBuffer() {}

  /**
   * @brief Get the unique name of the buffer generated by the Rendering API.
   *
   * @return The unique name of the buffer generated by the Rendering API.
   */
  virtual inline uint32_t name() const { return buffer_ID_; }
  /**
   * @brief Get the number of elements in the buffer. The total size in bytes
   * divided by the size in bytes of the data type used to create the buffer.
   *
   * @return The number of elements in the buffer
   */
  inline const uint32_t count() const { return count_; }
  /**
   * @brief Get the total size in bytes of the buffer.
   *
   * @return The size in bytes of the buffer.
   */
  inline const size_t size() const { return size_; }
  /**
   * @brief Get the current index of the buffer swap chain.
   * @details If the buffer is not double or triple buffered, this is always 0.
   * Otherwise, it is the current index into the buffer's swap chain. This is
   * updated when the buffers are swaped.
   *
   * @return The current index of the buffer swap chain.
   */
  inline const uint8_t buffer_index() const { return buffer_index_; }
  /**
   * @brief Returns the type of the buffer.
   *
   * @return The BufferType used by the buffer.
   * @see BufferType
   */
  inline const BufferType type() const { return type_; }
  /**
   * @brief Swaps the buffer to the next buffer in the swap chain. If single
   * buffered, nothing happens.
   */
  inline void swap_buffer() {
    buffer_index_ = (buffer_index_ + 1) % num_buffers_;
  }
  /**
   * @brief Get the current BufferFormat.
   *
   * @return The Buffer's BufferFormat.
   */
  inline BufferFormat &format() { return format_; }

  /**
   * @brief Wait for any locks to be released on the buffer before
   * proceeding. Used only on multibuffered buffers before writing data.
   */
  virtual void wait_buffer() = 0;
  /**
   * @brief Place a lock on the buffer. Any previously submitted buffer
   * operations will need to be completed before the lock is released. Used only
   * on multibuffered buffers.
   */
  virtual void lock_buffer() = 0;

protected:
  uint32_t buffer_ID_; /**< Unique Buffer ID used by the Rendering API.*/
  uint32_t
      count_;   /**< number of elements in the buffer of the buffer data type.*/
  size_t size_; /**< size in bytes of the buffer.*/
  BufferType type_; /**< The BufferType of the buffer. Determines the storage
                       and access properties of the Buffer.*/
  uint8_t buffer_index_; /**< Index of the currently buffer in the swap chain.*/
  uint8_t num_buffers_; /**< number of buffers in the swap chain - larger than 1
                           if multibuffered.*/
  BufferFormat format_; /**< The BufferFormat of the buffered data*/
};

/**
 * @brief A template class for storing data in a Buffer managed by the Rendering
 * API.
 * @details This is an abstract class that is implemented by the rendering API.
 *
 * @tparam <T> The type of data stored in the buffer.
 * @see Renderer
 * @see GLBuffer
 * @see GLRenderer
 * @see BufferType
 */
template <typename T> class Buffer : public IBuffer {
public:
  using IBuffer::buffer_ID_;
  using IBuffer::buffer_index_;
  using IBuffer::count_;
  using IBuffer::format_;
  using IBuffer::num_buffers_;
  using IBuffer::size_;
  using IBuffer::type_;
  /**
   * @brief Construct a new base Buffer object.
   * @details Buffer is an interface that is implemented by the Rendering API.
   * The constructor is only used by the constructor of the Buffer class
   * implemented by the Rendering API.
   *
   * @param data A pointer to the data used to initialize the buffer. Can be
   * nullptr is BufferType is not BufferType::STATIC.
   * @param size_in_bytes The size in bytes of the buffer. This is the size of a
   * single buffer if the BufferType is multibuffered.
   * @param buffer_type Specifies the access and storage properties of the
   * Buffer.
   * @see BufferType
   * @see GLBuffer
   */
  Buffer(T *data, size_t size_in_bytes,
         BufferType buffer_type = BufferType::STATIC) {
    buffer_ID_ = 0;
    type_ = buffer_type;
    buffer_index_ = 0;
    num_buffers_ = 1;
    size_ = size_in_bytes;
    count_ = data ? static_cast<uint32_t>(size_in_bytes / sizeof(T)) : 0;

    if (buffer_type == BufferType::STATIC && !data) {
      std::cerr << "BUFFER ERROR: Static buffer needs to be initialized on "
                   "construction!"
                << std::endl;
    }

    switch (buffer_type) {
    case BufferType::WRITE_ONLY_DOUBLE_BUFFERED:
      num_buffers_ = 2;
      break;
    case BufferType::READ_WRITE_DOUBLE_BUFFERED:
      num_buffers_ = 2;
      break;
    case BufferType::WRITE_ONLY_TRIPLE_BUFFERED:
      num_buffers_ = 3;
      break;
    case BufferType::READ_WRITE_TRIPLE_BUFFERED:
      num_buffers_ = 3;
      break;
    default:
      break;
    }
  }
  /**
   * @brief Virtual destructor of the base Buffer object.
   *
   */
  virtual ~Buffer() {}

  /**
   * @brief Set the format of the Buffer
   *
   * @param format The BufferFormat to be set.
   * @see BufferFormat
   */
  inline void set_format(BufferFormat format) {
    format_ = format;
    format_.stride = 0;
    size_t offset = 0;
    for (auto &attrib : format_.attributes()) {
      attrib.offset = offset;
      offset += sizeof(T) * attrib.component_count();
      format_.stride += sizeof(T) * attrib.component_count();
    }
    if (format_.stride) {
      count_ = static_cast<uint32_t>(size_ / format_.stride);
    } else {
      count_ = 0;
    }
  }

  /**
   * @brief Flush the Buffer with new data.
   * @details The BufferType must allow write access to the Buffer. If the
   * Buffer is multibuffered, only the active buffer in the swap chain is
   * written to.
   *
   * @param data A pointer to the data to flush the buffer with.
   * @param offset_index The index into the buffer where the new data will
   * start. Must be less that the count of the buffer elements.
   * @param size_in_bytes The size in bytes of the data to be written.
   */
  virtual void flush(T *data, uint32_t offset_index, size_t size_in_bytes) = 0;
  /**
   * @brief Provides write access to the buffer using the subscript operator.
   * @details BufferType must allow for writing to the Buffer.
   * @param i The index into the Buffer to write the data.
   * @return <T>& A reference into the buffer at the provided index.
   */
  virtual T &operator[](uint32_t i) = 0;
  /**
   * @brief Provides read access to the buffer using the subscript operator.
   * @details BufferType must allow for reading from the Buffer. This makes a
   * copy of the data on the client-side where the CPU can use it.
   * @param i The index into the Buffer to read data from.
   * @return <T> The data read from the buffer at the provided index.
   */
  virtual T operator[](uint32_t i) const = 0;
  /**
   * @brief Clear the entire Buffer to a uniform value.
   *
   * @param value The value used to clear the Buffer.
   */
  virtual void clear(T value) = 0;
};

/**
 * @brief The TextureType describes the format of the data used to create a
 * Texture object.
 */
enum class TextureType {
  R8,     /**< The Texture data is formated as single channel 8-bit bytes.*/
  RG8,    /**< The Texture data is formated as two channel 8-bit bytes.*/
  RGB8,   /**< The Texture data is formated as three channel 8-bit bytes.*/
  RGBA8,  /**< The Texture data is formated as four channel 8-bit bytes.*/
  R32F,   /**< The Texture data is formated as single channel 32-bit floating
             point numbers.*/
  RG32F,  /**< The Texture data is formated as two channel 32-bit floating point
             numbers.*/
  RGB32F, /**< The Texture data is formated as three channel 32-bit floating
             point numbers.*/
  RGBA32F, /**< The Texture data is formated as four channel 32-bit floating
              point numbers.*/
  DEPTH    /**< The Texture data is formated as single channel 32-bit floating
              point numbers. And used to create textures of the depth values from a
              Framebuffer.*/
};
/**
 * @brief A 2-Dimensional Texture managed by the Rendering API.
 * @details Reads an image file and creates a Texture2D which can be uploaded to
 * a Shader and used for rendering or as general purpose data storage. This is
 * an abastract class that is implemented by the Rendering API.
 * @see GLTexture2D
 */
class Texture2D {
public:
  /**
   * @brief Construct a new Texture2D object from an existing image file.
   * @details This constructor is used by the implemented Rendering API.
   * stb_image.h header file is used to read the image files. From the stb_image
   * documentation supported file types are:
   *  - JPEG baseline & progressive (12 bpc/arithmetic not supported, same as
   *  stock IJG lib)
   *  - PNG 1/2/4/8/16-bit-per-channel
   *  - TGA (not sure what subset, if a subset)
   *  - BMP non-1bpp, non-RLE
   *  - PSD (composited view only, no extra channels, 8/16 bit-per-channel)
   *  - GIF (*comp always reports as 4-channel)
   *  - HDR (radiance rgbE format)
   *  - PIC (Softimage PIC)
   *  - PNM (PPM and PGM binary only)
   *
   * @param filepath The filepath to the image used to create the Texture2D. The
   * path can be absolute or relative to the directory where the application is
   executed from.
   */
  Texture2D(const char *filepath) {}
  /**
   * @brief Construct a new uninitialized Texture2D object.
   * @details This constructor is used by the implemented Rendering API.
   * @param type The TextureType that specifies the storage properties of the
   * Texture2D.
   * @param width The width in pixels of the created texture.
   * @param height The height in pixels of the created texture.
   */
  Texture2D(TextureType type, int width, int height)
      : type_(type), width_(width), height_(height) {}
  /**
   * @brief Virtual destructor of the Texture2D object
   *
   */
  virtual ~Texture2D() = 0;
  /**
   * @brief Get the unique name of the Texture generated by the Rendering API.
   *
   * @return The unique name of the Texture.
   */
  inline uint32_t name() const { return texture_ID_; }
  /**
   * @brief Get the TextureType of the Texture.
   *
   * @return The TextureType of the Texture.
   */
  inline TextureType type() const { return type_; }

protected:
  uint32_t texture_ID_; /**< The unique ID of the texture generated by the
                           Rendering API.*/
  TextureType type_;    /**< The TextureType of the Texture.*/
  int width_;           /**< The width of the Texture in pixels.*/
  int height_;          /**< The height of the Texture in pixels.*/
  int channels_;        /**< The number of channels in the Texture image.*/
  Scoped<IBuffer>
      texture_buffer_; /**< The Buffer used to store the Texture data.*/
};

/**
 * @brief A Framebuffer object that can be used to render into.
 * @details The Framebuffer has a depth Texture and a color Texture used to
 * record information from the depth and color component of the Framebuffer. The
 * Framebuffer can be swapped with the default Framebuffer using
 * Renderer::set_framebuffer(Framebuffer*). This is an abstract class that is
 * implemented by the Rendering API.
 * @see Renderer
 * @see GLFramebuffer
 */
class Framebuffer {
public:
  /**
   * @brief Construct a new base Framebuffer object.
   * @details This is only used by the implementation of the Rendering API
   * Framebuffer.
   *
   * @param width The width in pixels of the Framebuffer.
   * @param height The height in pixels of the Framebuffer.
   */
  Framebuffer(int width, int height) {}
  /**
   * @brief Virtual destructor of the Framebuffer object
   *
   */
  virtual ~Framebuffer() = 0;
  /**
   * @brief Get the unique name of the Framebuffer generated by the Rendering
   * API.
   *
   * @return The unique name of the Framebuffer generated by the Rendering API.
   */
  inline uint32_t name() const { return framebuffer_ID_; }
  /**
   * @brief Get a pointer to the depth texture of the Framebuffer.
   *
   * @return A pointer to the depth texture of the Framebuffer
   */
  inline Texture2D *depth_texture() { return depth_texture_.get(); }
  /**
   * @brief Get a pointer to the color texture of the Framebuffer.
   *
   * @return A pointer to the color texture of the Framebuffer
   */
  inline Texture2D *color_texture() { return color_texture_.get(); }

protected:
  uint32_t framebuffer_ID_; /**< The unique ID of the Framebuffer generated by
                               the Rendering API.*/
  Scoped<Texture2D> color_texture_; /**< A Scoped reference to the color texture
                                       of the Framebuffer.*/
  Scoped<Texture2D> depth_texture_; /**< A Scoped reference to the depth texture
                                       of the Framebuffer.*/
};
// --------------------------------------------------------------------------------------------------------
} // namespace mare

#endif