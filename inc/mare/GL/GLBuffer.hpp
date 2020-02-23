#ifndef GLBUFFER
#define GLBUFFER

// MARE
#include "mare/Buffer.hpp"
#include "mare/MareUtility.hpp"

// OpenGL
#include "GL/glew.h"

namespace mare
{
template <typename T>
class GLBuffer : public Buffer<T>
{
public:
    GLBuffer(std::vector<T> *data, BufferType buffer_type = BufferType::STATIC, size_t size_in_bytes = 0);
    virtual ~GLBuffer();
    virtual void flush(std::vector<T> &data, size_t offset) override;
    virtual void clear(unsigned int offset) override;
    virtual T &operator[](unsigned int i) override;
    virtual T operator[](unsigned int i) const override;
    void wait_buffer() override;
    void lock_buffer() override;

private:
    T *buffer_pointer;
    GLsync* buffer_fence; // used to sync rendering when double or triple buffered
};

} // namespace mare

#endif