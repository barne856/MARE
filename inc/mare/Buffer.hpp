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
    BufferFormatElement(LinalgDataType type, std::string name, bool normalized = false);
    size_t component_count() const;
    std::string name;
    size_t size;
    size_t offset;
    bool normalized;
    LinalgDataType data_type;
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
    unsigned int stride;

private:
    std::vector<BufferFormatElement> m_elements;
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
    const unsigned int count() const;
    unsigned int name() const;
    void swap_buffer();
    bool is_multibuffered();
    unsigned int get_buffer_index();
    virtual void flush(std::vector<T> &data, unsigned int offset) = 0; // flush the back buffer with data begining at offset index into the current back buffer, only works if buffer is WRITE_ONLY or READ_WRITE
    virtual void clear(unsigned int offset) = 0;                       // clear everything in the back buffer at position equal to or greater than offset into the current back buffer, only works if buffer is WRITE_ONLY or READ_WRITE
    virtual T &operator[](unsigned int i) = 0;                         // write to back buffer using subscript operator if buffer WRITE_ONLY or READ_WRITE. This makes a copy of the data on the CPU side
    virtual T operator[](unsigned int i) const = 0;                    // read from to back buffer using subscript operator if buffer READ_ONLY or READ_WRITE
    virtual void wait_buffer() = 0;                                    // used to sync rendering when using double or triple buffers
    virtual void lock_buffer() = 0;                                    // used to sync rendering when using double or triple buffers

protected:
    unsigned int m_buffer_ID;      // Buffer ID from the rendering API
    BufferFormat m_format;         // the applied format is any
    size_t m_count;                // the number of elements in the buffer
    size_t m_data_size;            // the size in bytes of all the elements in the buffer
    BufferType m_buffer_type;      // buffer type, this determines which operations the CPU can do on the buffer
    unsigned short m_buffer_index; // the currently rendered buffer if multibuffered
    unsigned short m_num_buffers;  // the number of buffers if multibuffered
};
} // namespace mare

#endif