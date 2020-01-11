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
    virtual void create(std::vector<T> &data) override;
    virtual void update(std::vector<T> &data, unsigned int offset) override;

    static unsigned int gl_type(ShaderDataType type);
};

} // namespace mare

#endif