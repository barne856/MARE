#include "mare/Buffer.hpp"
#include <iostream>

namespace mare
{

// BufferFormat Element
BufferFormatElement::BufferFormatElement(LinalgDataType type, std::string name, bool normalized)
    : data_type(type), name(name), size(linalg_data_type_size(type)), offset(0), normalized(normalized) {}

size_t BufferFormatElement::component_count() const
{
    return linalg_component_count(data_type);
}

// BufferFormat
BufferFormat::BufferFormat() : m_elements({}), stride(0) {}
BufferFormat::BufferFormat(const std::initializer_list<BufferFormatElement> &elements)
    : m_elements(elements), stride(0)
{
    uint32_t offset = 0;
    for (auto &e : m_elements)
    {
        e.offset = offset;
        offset += e.size;
        stride += e.size;
    }
}
const std::vector<BufferFormatElement> &BufferFormat::elements() const { return m_elements; }
std::vector<BufferFormatElement>::iterator BufferFormat::begin() { return m_elements.begin(); }
std::vector<BufferFormatElement>::iterator BufferFormat::end() { return m_elements.end(); }
std::vector<BufferFormatElement>::const_iterator BufferFormat::begin() const { return m_elements.begin(); }
std::vector<BufferFormatElement>::const_iterator BufferFormat::end() const { return m_elements.end(); }

// Buffer
template <typename T>
Buffer<T>::Buffer(std::vector<T> *data, BufferType buffer_type, size_t size_in_bytes)
    : m_buffer_ID(0), m_format(BufferFormat()), m_buffer_type(buffer_type), m_buffer_index(0), m_num_buffers(0)
{
    m_count = data ? data->size() : 0;
    m_data_size = size_in_bytes ? size_in_bytes : sizeof(T) * m_count;

    // if there is no data supplied
    if (!data)
    {
        // and buffer is static
        if (buffer_type == BufferType::STATIC)
        {
            std::cerr << "ERROR: Static buffer needs to be initialized on construction!" << std::endl;
        }
        // or size is zero
        else if (size_in_bytes == 0)
        {
            std::cerr << "ERROR: Dynamic buffer needs to be allocated on construction!" << std::endl;
        }
    }
    // else data is supplied and size in bytes is non zero
    else if (size_in_bytes != 0)
    {
        // and buffer is static
        if (buffer_type == BufferType::STATIC)
        {
            std::cerr << "WARNING: Static buffer cannot have a size_in_bytes parameter supplied, size will be taken from the size of the given data." << std::endl;
        }
        // or buffer is dynamic and size in bytes is smaller than the given data size
        else if (size_in_bytes < m_data_size)
        {
            std::cerr << "WARNING: size_in_bytes parameter is smaller than the given data size, allocated size will truncated." << std::endl;
        }
    }

    switch (buffer_type)
    {
    case BufferType::READ_ONLY_DOUBLE_BUFFERED:
        m_num_buffers = 2;
        break;
    case BufferType::WRITE_ONLY_DOUBLE_BUFFERED:
        m_num_buffers = 2;
        break;
    case BufferType::READ_WRITE_DOUBLE_BUFFERED:
        m_num_buffers = 2;
        break;
    case BufferType::READ_ONLY_TRIPLE_BUFFERED:
        m_num_buffers = 3;
        break;
    case BufferType::WRITE_ONLY_TRIPLE_BUFFERED:
        m_num_buffers = 3;
        break;
    case BufferType::READ_WRITE_TRIPLE_BUFFERED:
        m_num_buffers = 3;
        break;

    default:
        break;
    }
}
template <typename T>
void Buffer<T>::set_format(const BufferFormat &format)
{
    m_format = format;
    m_count = m_data_size / format.stride;
}
template <typename T>
const BufferFormat &Buffer<T>::format() const { return m_format; }
template <typename T>
const unsigned int Buffer<T>::count() const { return m_count; }
template <typename T>
unsigned int Buffer<T>::name() const { return m_buffer_ID; }

template <typename T>
void Buffer<T>::swap_buffer()
{
    if (m_num_buffers)
    {
        m_buffer_index = (m_buffer_index + 1) % m_num_buffers;
    }
}
template <typename T>
bool Buffer<T>::is_multibuffered()
{
    // if single buffered m_num_buffers == 0
    return static_cast<bool>(m_num_buffers);
}
template <typename T>
unsigned int Buffer<T>::get_buffer_index()
{
    return m_buffer_index;
}

template class Buffer<float>;
template class Buffer<int>;
template class Buffer<unsigned int>;
template class Buffer<bool>;
template class Buffer<glm::vec2>;
template class Buffer<glm::vec3>;
template class Buffer<glm::vec4>;
template class Buffer<glm::mat2>;
template class Buffer<glm::mat3>;
template class Buffer<glm::mat4>;

} // namespace mare