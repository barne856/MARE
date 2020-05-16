#ifndef BUFFERS
#define BUFFERS

// Standard Library
#include <vector>
#include <string>
#include <iostream>

// MARE
#include "mare/Mare.hpp"

namespace mare
{
    // GENERIC BUFFER -----------------------------------------------------------------------------------------
    enum class BufferType
    {
        STATIC = 0,
        READ_ONLY,
        WRITE_ONLY,
        READ_WRITE,
        WRITE_ONLY_DOUBLE_BUFFERED,
        WRITE_ONLY_TRIPLE_BUFFERED,
        READ_WRITE_DOUBLE_BUFFERED,
        READ_WRITE_TRIPLE_BUFFERED
    };
    enum class Attribute
    {
        POSITION_2D,
        POSITON_3D,
        NORMAL,
        TEXTURE_MAP,
        TEXTURE_2D,
        UNIFORM,
        STORAGE
    };
    // Individual elements in the buffer format
    // Each element corresponds to an input to a shader of the type given
    class BufferAttibrute
    {
    public:
        BufferAttibrute(Attribute attrib_type, std::string attrib_name) : type(attrib_type), name(attrib_name), offset(0), size(0) {}
        ~BufferAttibrute(){};
        const uint32_t component_count() const
        {
            switch (type)
            {
            case Attribute::POSITION_2D:
                return 2;
                break;
            case Attribute::POSITON_3D:
                return 3;
                break;
            case Attribute::NORMAL:
                return 3;
                break;
            case Attribute::TEXTURE_MAP:
                return 2;
                break;
            default:
                return 0;
                break;
            }
        }
        Attribute type;
        std::string name;
        size_t offset;
        size_t size;
    };

    // A BufferFormat, when applied to a Buffer will tell the shader how the information is stored
    // in the buffer, i.e. interlaced or stacked, offsets, etc.
    // Each Buffer can have a Format with one or more elements or muliple Buffers
    // can be used each with their own BufferFormat
    class BufferFormat
    {
    public:
        BufferFormat() : attributes_({}), stride(0) {}
        BufferFormat(const std::initializer_list<BufferAttibrute> &attributes)
            : attributes_(attributes) {}
        std::vector<BufferAttibrute> &attributes() { return attributes_; }
        std::vector<BufferAttibrute>::iterator begin() { return attributes_.begin(); }
        std::vector<BufferAttibrute>::iterator end() { return attributes_.end(); }
        std::vector<BufferAttibrute>::const_iterator begin() const { return attributes_.begin(); }
        std::vector<BufferAttibrute>::const_iterator end() const { return attributes_.end(); }
        size_t stride;

    private:
        std::vector<BufferAttibrute> attributes_;
    };

    // Generic template for buffers managed by the rendering API
    class IBuffer
    {
    public:
        IBuffer() {}
        virtual ~IBuffer() {}

        virtual inline uint32_t name() const { return buffer_ID_; }
        inline const uint32_t count() const { return count_; }
        inline const size_t size() const { return size_; }
        inline const uint8_t buffer_index() const { return buffer_index_; }
        inline const BufferType type() const { return type_; }
        inline void swap_buffer() { buffer_index_ = (buffer_index_ + 1) % num_buffers_; }
        inline BufferFormat &format() { return format_; }

        virtual void wait_buffer() = 0;                // used to sync rendering when using double or triple buffers
        virtual void lock_buffer() = 0;                // used to sync rendering when using double or triple buffers
    protected:
        uint32_t buffer_ID_;   // ID used by the rendering API
        uint32_t count_;       // number of entries in the buffer
        size_t size_;          // size in bytes of the buffer
        BufferType type_;      // the type of buffer - determines which operations the CPU can do with the buffer
        uint8_t buffer_index_; // index of the currently rendered buffer
        uint8_t num_buffers_;  // number of buffers - larger than 1 if multibuffered
        BufferFormat format_;  // format for the buffer's data
    };
    template <typename T>
    class Buffer : public IBuffer
    {
    public:
        using IBuffer::buffer_ID_;
        using IBuffer::buffer_index_;
        using IBuffer::count_;
        using IBuffer::format_;
        using IBuffer::num_buffers_;
        using IBuffer::size_;
        using IBuffer::type_;
        // size_in_bytes is the size of a single buffer if buffer is multibuffered
        Buffer(T *data, size_t size_in_bytes, BufferType buffer_type = BufferType::STATIC)
        {
            buffer_ID_ = 0;
            type_ = buffer_type;
            buffer_index_ = 0;
            num_buffers_ = 1;
            size_ = size_in_bytes;
            assert(size_in_bytes > 0); // Buffer size must be larger than 0. Storage size is immuatable
            count_ = data ? static_cast<uint32_t>(size_in_bytes / sizeof(T)) : 0;

            if (buffer_type == BufferType::STATIC && !data)
            {
                std::cerr << "BUFFER ERROR: Static buffer needs to be initialized on construction!" << std::endl;
            }

            switch (buffer_type)
            {
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
        virtual ~Buffer() {}

        inline void set_format(BufferFormat format)
        {
            format_ = format;
            format_.stride = 0;
            size_t offset = 0;
            for (auto &attrib : format_.attributes())
            {
                attrib.offset = offset;
                offset += sizeof(T) * attrib.component_count();
                format_.stride += sizeof(T) * attrib.component_count();
            }
            if (format_.stride)
            {
                count_ = static_cast<uint32_t>(size_ / format_.stride);
            }
            else
            {
                count_ = 0;
            }
        }

        virtual void flush(T *data, uint32_t offset_index, size_t size_in_bytes) = 0; // READ_WRITE or WRITE_ONLY - flush the back buffer with data
        virtual T &operator[](uint32_t i) = 0;                                        // write to back buffer using subscript operator if buffer WRITE_ONLY or READ_WRITE. This makes a copy of the data on the CPU side
        virtual T operator[](uint32_t i) const = 0;                                   // read from to back buffer using subscript operator if buffer READ_ONLY or READ_WRITE
        virtual void clear(T value) = 0;                                              // clear the buffer to a value
    };
    // --------------------------------------------------------------------------------------------------------

    // TEXTURE BUFFER -----------------------------------------------------------------------------------------
    // Buffers for storage of a texture sampler in a glsl shader
    enum class TextureType
    {
        R8,
        RG8,
        RGB8,
        RGBA8,
        R32F,
        RG32F,
        RGB32F,
        RGBA32F,
        DEPTH
    };
    class Texture2D
    {
    public:
        Texture2D(const char *filepath) {}
        Texture2D(TextureType type, int width, int height) : type_(type), width_(width), height_(height) {}
        virtual ~Texture2D() = 0;
        inline uint32_t name() const { return texture_ID_; }
        inline TextureType type() const { return type_; }

    protected:
        uint32_t texture_ID_;
        TextureType type_;
        int width_, height_, channels_;
        Scoped<IBuffer> texture_buffer_;
    };
    // --------------------------------------------------------------------------------------------------------

    // FRAME BUFFER -------------------------------------------------------------------------------------------
    // Buffer fdor storage of a framebuffer with color and depth components
    class Framebuffer
    {
    public:
        Framebuffer(int width, int height) {}
        virtual ~Framebuffer() = 0;
        inline uint32_t name() const { return framebuffer_ID_; }
        inline Texture2D *depth_texture() { return depth_texture_.get(); }
        inline Texture2D *color_texture() { return color_texture_.get(); }

    protected:
        uint32_t framebuffer_ID_;
        Scoped<Texture2D> color_texture_;
        Scoped<Texture2D> depth_texture_;
    };
    // --------------------------------------------------------------------------------------------------------
} // namespace mare

#endif