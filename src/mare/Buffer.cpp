#include "mare/Buffer.hpp"
#include <iostream>

namespace mare
{

// BufferFormat Element
BufferFormatElement::BufferFormatElement(ShaderDataType type, std::string name, bool normalized)
    : data_type(type), name(name), size(shader_data_type_size(type)), offset(0), normalized(normalized) {}

size_t BufferFormatElement::component_count() const
{
    return shader_component_count(data_type);
}

// BufferFormat
BufferFormat::BufferFormat() : elements_({}), stride(0) {}
BufferFormat::BufferFormat(const std::initializer_list<BufferFormatElement> &elements)
    : elements_(elements), stride(0)
{
    size_t offset = 0;
    for (auto &e : elements_)
    {
        e.offset = offset;
        offset += e.size;
        stride += e.size;
    }
}
const std::vector<BufferFormatElement> &BufferFormat::elements() const { return elements_; }
std::vector<BufferFormatElement>::iterator BufferFormat::begin() { return elements_.begin(); }
std::vector<BufferFormatElement>::iterator BufferFormat::end() { return elements_.end(); }
std::vector<BufferFormatElement>::const_iterator BufferFormat::begin() const { return elements_.begin(); }
std::vector<BufferFormatElement>::const_iterator BufferFormat::end() const { return elements_.end(); }

// Buffer
template <typename T>
Buffer<T>::Buffer(std::vector<T> *data, BufferType buffer_type, size_t size_in_bytes)
    : buffer_ID_(0), format_(BufferFormat()), buffer_type_(buffer_type), buffer_index_(0), num_buffers_(0)
{
    count_ = data ? data->size() : 0;
    data_size_ = size_in_bytes ? size_in_bytes : sizeof(T) * count_;

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
        else if (size_in_bytes < data_size_)
        {
            std::cerr << "WARNING: size_in_bytes parameter is smaller than the given data size, allocated size will truncated." << std::endl;
        }
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
template <typename T>
void Buffer<T>::set_format(const BufferFormat &format)
{
    format_ = format;
    count_ = data_size_ / format.stride;
}
template <typename T>
const BufferFormat &Buffer<T>::format() const { return format_; }
template <typename T>
const size_t Buffer<T>::count() const { return count_; }
template <typename T>
unsigned int Buffer<T>::name() const { return buffer_ID_; }

template <typename T>
void Buffer<T>::swap_buffer()
{
    // m_num_buffers is zero if single buffered
    if (num_buffers_)
    {
        buffer_index_ = (buffer_index_ + 1) % num_buffers_;
    }
}
template <typename T>
bool Buffer<T>::is_multibuffered()
{
    // if single buffered m_num_buffers == 0
    return static_cast<bool>(num_buffers_);
}
template <typename T>
unsigned int Buffer<T>::get_buffer_index()
{
    return buffer_index_;
}

template class Buffer<float>;
template class Buffer<int>;
template class Buffer<unsigned int>;
template class Buffer<glm::vec2>;
template class Buffer<glm::vec3>;
template class Buffer<glm::vec4>;
template class Buffer<glm::mat2>;
template class Buffer<glm::mat3>;
template class Buffer<glm::mat4>;

} // namespace mare