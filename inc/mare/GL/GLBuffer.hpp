#ifndef GLBUFFER
#define GLBUFFER

// MARE
#include "mare/Buffer.hpp"

namespace mare
{
template <typename T>
class GLBuffer : public Buffer<T>
{
public:
    virtual ~GLBuffer();
    virtual void create(std::vector<T>& data, size_t dynamic_size_in_bytes = 0) override;
    virtual void create(T *data, size_t size_in_bytes, size_t dynamic_size_in_bytes = 0) override;
    virtual void update(std::vector<T>& data, unsigned int offset) override;
    virtual void update(T *data, size_t size_in_bytes, unsigned int offset) override;

    static unsigned int gl_type(ShaderDataType type);
};

} // namespace mare

#endif