#ifndef BUFFER
#define BUFFER

// Standard Library
#include <vector>
#include <string>

// External Libraries
#include "glm.hpp"

namespace mare
{
enum class ShaderDataType
{
    NONE = 0,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    MAT2,
    MAT3,
    MAT4,
    INT,
    INT2,
    INT3,
    INT4,
    BOOL,
    CHAR,
    BYTE,
    SHORT,
    UNSIGNED_SHORT,
    UNSIGNED_INT
};

template <typename T>
ShaderDataType glm_to_shader_type();

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
        case ShaderDataType::FLOAT:
            return 4;
        case ShaderDataType::VEC2:
            return 4 * 2;
        case ShaderDataType::VEC3:
            return 4 * 3;
        case ShaderDataType::VEC4:
            return 4 * 4;
        case ShaderDataType::MAT2:
            return 4 * 2 * 2;
        case ShaderDataType::MAT3:
            return 4 * 3 * 3;
        case ShaderDataType::MAT4:
            return 4 * 4 * 4;
        case ShaderDataType::INT:
            return 4;
        case ShaderDataType::INT2:
            return 4 * 2;
        case ShaderDataType::INT3:
            return 4 * 3;
        case ShaderDataType::INT4:
            return 4 * 4;
        case ShaderDataType::BOOL:
            return 1;
        case ShaderDataType::CHAR:
            return 1;
        case ShaderDataType::BYTE:
            return 1;
        case ShaderDataType::UNSIGNED_SHORT:
            return 2;
        case ShaderDataType::SHORT:
            return 2;
        case ShaderDataType::UNSIGNED_INT:
            return 4;
        default:
            return 0;
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
        case ShaderDataType::FLOAT:
            return 1;
        case ShaderDataType::VEC2:
            return 2;
        case ShaderDataType::VEC3:
            return 3;
        case ShaderDataType::VEC4:
            return 4;
        case ShaderDataType::MAT2:
            return 2 * 2;
        case ShaderDataType::MAT3:
            return 3 * 3;
        case ShaderDataType::MAT4:
            return 4 * 4;
        case ShaderDataType::INT:
            return 1;
        case ShaderDataType::INT2:
            return 2;
        case ShaderDataType::INT3:
            return 3;
        case ShaderDataType::INT4:
            return 4;
        case ShaderDataType::BOOL:
            return 1;
        case ShaderDataType::CHAR:
            return 1;
        case ShaderDataType::BYTE:
            return 1;
        case ShaderDataType::UNSIGNED_SHORT:
            return 1;
        case ShaderDataType::SHORT:
            return 1;
        case ShaderDataType::UNSIGNED_INT:
            return 1;
        default:
            return 0;
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
    Buffer() : m_buffer_ID(0), m_format(BufferFormat()), m_count(0), m_data_size(0), m_dynamic_size_in_bytes(0), is_dynamic(false) {}
    virtual ~Buffer() {}
    void inline set_format(const BufferFormat &format)
    {
        m_format = format;
        m_count = unsigned int(m_data_size / format.stride());
    }
    virtual void create(std::vector<T>& data, size_t dynamic_size_in_bytes = 0) = 0;
    virtual void create(T *data, size_t size_in_bytes, size_t dynamic_size_in_bytes = 0) = 0;
    virtual void update(std::vector<T>& data, unsigned int offset) = 0;
    virtual void update(T *data, size_t size_in_bytes, unsigned int offset) = 0;
    inline const unsigned int count() const { return m_count; }
    inline unsigned int name() { return m_buffer_ID; }
    inline const BufferFormat &format() const { return m_format; }

protected:
    unsigned int m_buffer_ID;
    BufferFormat m_format;
    unsigned int m_count;
    size_t m_data_size;
    size_t m_dynamic_size_in_bytes;
    bool is_dynamic;
};
} // namespace mare

#endif