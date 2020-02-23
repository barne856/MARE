#ifndef BUFFER
#define BUFFER

// Standard Library
#include <vector>
#include <string>

// External Libraries
#include "glm.hpp"

// MARE
#include "mare/MareUtility.hpp"

namespace mare
{
// Individual Elements in the buffer format
// Each element corresponds to an input to a shader of the type given
class BufferFormatElement
{
public:
    BufferFormatElement(ShaderDataType type, std::string name, bool normalized = false);
    size_t component_count() const;
    std::string name;
    size_t size;
    size_t offset;
    bool normalized;
    ShaderDataType data_type;
};

// A BufferFormat, when applied to a Buffer will tell the shader how the information is stored
// in the buffer, i.e. interlaced or stacked, offsets, etc.
// Each Buffer can have a Format with one or more elements or muliple Buffers
// can be used each with their own BufferFormat
class BufferFormat
{
public:
    BufferFormat();
    BufferFormat(const std::initializer_list<BufferFormatElement> &elements);
    const std::vector<BufferFormatElement> &elements() const;
    std::vector<BufferFormatElement>::iterator begin();
    std::vector<BufferFormatElement>::iterator end();
    std::vector<BufferFormatElement>::const_iterator begin() const;
    std::vector<BufferFormatElement>::const_iterator end() const;
    size_t stride;

private:
    std::vector<BufferFormatElement> elements_;
};

enum class BufferType
{
    STATIC = 0,
    READ_ONLY,
    READ_ONLY_DOUBLE_BUFFERED,
    READ_ONLY_TRIPLE_BUFFERED,
    READ_WRITE,
    READ_WRITE_DOUBLE_BUFFERED,
    READ_WRITE_TRIPLE_BUFFERED,
    WRITE_ONLY,
    WRITE_ONLY_DOUBLE_BUFFERED,
    WRITE_ONLY_TRIPLE_BUFFERED
};

// Create a buffer managed by the Graphics API
// These can be used to push data to a shader as attributes using a RenderState or as storage blocks
template <typename T>
class Buffer
{
public:
    // Create a buffer
    // size_in_bytes is the size of a single buffer if the buffer is multibuffered
    Buffer(std::vector<T> *data, BufferType buffer_type = BufferType::STATIC, size_t size_in_bytes = 0);
    virtual ~Buffer() {}
    void set_format(const BufferFormat &format);
    const BufferFormat &format() const;
    const size_t count() const;
    unsigned int name() const;
    void swap_buffer();
    bool is_multibuffered();
    unsigned int get_buffer_index();
    virtual void flush(std::vector<T> &data, size_t offset) = 0; // flush the back buffer with data begining at offset index into the current back buffer, only works if buffer is WRITE_ONLY or READ_WRITE
    virtual void clear(unsigned int offset) = 0;                       // clear everything in the back buffer at position equal to or greater than offset into the current back buffer, only works if buffer is WRITE_ONLY or READ_WRITE
    virtual T &operator[](unsigned int i) = 0;                         // write to back buffer using subscript operator if buffer WRITE_ONLY or READ_WRITE. This makes a copy of the data on the CPU side
    virtual T operator[](unsigned int i) const = 0;                    // read from to back buffer using subscript operator if buffer READ_ONLY or READ_WRITE
    virtual void wait_buffer() = 0;                                    // used to sync rendering when using double or triple buffers
    virtual void lock_buffer() = 0;                                    // used to sync rendering when using double or triple buffers

protected:
    unsigned int buffer_ID_;      // Buffer ID from the rendering API
    BufferFormat format_;         // the applied format is any
    size_t count_;                // the number of elements in the buffer
    size_t data_size_;            // the size in bytes of all the elements in the buffer
    BufferType buffer_type_;      // buffer type, this determines which operations the CPU can do on the buffer
    unsigned short buffer_index_; // the currently rendered buffer if multibuffered
    unsigned short num_buffers_;  // the number of buffers if multibuffered
};
} // namespace mare

#endif