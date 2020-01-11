#ifndef BUFFER
#define BUFFER

// Standar Library
#include <vector>
#include <string>

namespace mare
{
enum class ShaderDataType
{
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

struct BufferFormatElement
{

    std::string m_name;
    unsigned int m_size;
    unsigned int m_offset;
    ShaderDataType m_type;
    bool m_normalized;

    BufferFormatElement() {}
    BufferFormatElement(ShaderDataType type, std::string name, bool normalized = false)
        : m_name(name), m_type(type), m_size(ShaderDataTypeSize(type)), m_offset(0), m_normalized(normalized) {}

    static unsigned int ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
            return 4;
        case ShaderDataType::Float2:
            return 4 * 2;
        case ShaderDataType::Float3:
            return 4 * 3;
        case ShaderDataType::Float4:
            return 4 * 4;
        case ShaderDataType::Mat3:
            return 4 * 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4 * 4;
        case ShaderDataType::Int:
            return 4;
        case ShaderDataType::Int2:
            return 4 * 2;
        case ShaderDataType::Int3:
            return 4 * 3;
        case ShaderDataType::Int4:
            return 4 * 4;
        case ShaderDataType::Bool:
            return 1;
        }
        return 0;
    }
    ShaderDataType type() const
    {
        return m_type;
    }
    unsigned int component_count() const
    {
        switch (m_type)
        {
        case ShaderDataType::Float:
            return 1;
        case ShaderDataType::Float2:
            return 2;
        case ShaderDataType::Float3:
            return 3;
        case ShaderDataType::Float4:
            return 4;
        case ShaderDataType::Mat3:
            return 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4;
        case ShaderDataType::Int:
            return 1;
        case ShaderDataType::Int2:
            return 2;
        case ShaderDataType::Int3:
            return 3;
        case ShaderDataType::Int4:
            return 4;
        case ShaderDataType::Bool:
            return 1;
        }
        return 0;
    }
};

class BufferFormat
{
public:
    BufferFormat() {}
    BufferFormat(const std::initializer_list<BufferFormatElement> &elements)
        : m_elements(elements)
    {
        m_stride = 0;
        uint32_t offset = 0;
        for (auto &e : m_elements)
        {
            e.m_offset = offset;
            offset += e.m_size;
            m_stride += e.m_size;
        }
    }
    ~BufferFormat() {}
    inline const std::vector<BufferFormatElement> &elements() const { return m_elements; }
    inline const unsigned int stride() const { return m_stride; }
    std::vector<BufferFormatElement>::iterator begin() { return m_elements.begin(); }
    std::vector<BufferFormatElement>::iterator end() { return m_elements.end(); }
    std::vector<BufferFormatElement>::const_iterator begin() const { return m_elements.begin(); }
    std::vector<BufferFormatElement>::const_iterator end() const { return m_elements.end(); }

private:
    std::vector<BufferFormatElement> m_elements;
    unsigned int m_stride = 0;
};

template <typename T>
class Buffer
{
public:
    Buffer() : m_buffer_ID(0), m_format(BufferFormat()), m_count(0), m_data_size(0) {}
    virtual ~Buffer() {}
    void inline set_format(const BufferFormat &format)
    {
        m_format = format;
        m_count = m_data_size / format.stride();
    }
    virtual void create(std::vector<T> &data) = 0;
    virtual void update(std::vector<T> &data, unsigned int offset) = 0;
    inline const size_t count() const { return m_count; }
    inline unsigned int name() { return m_buffer_ID; }
    inline const BufferFormat &format() const { return m_format; }

protected:
    unsigned int m_buffer_ID;
    BufferFormat m_format;
    size_t m_count;
    size_t m_data_size;
};
} // namespace mare

#endif