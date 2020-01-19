#ifndef GLRENDERSTATE
#define GLRENDERSTATE

// MARE
#include "mare/RenderState.hpp"

// External Libraries
#include "glm.hpp"

namespace mare
{
template <typename T>
class GLRenderState : public RenderState<T>
{
public:
    GLRenderState() {}
    ~GLRenderState();
    void bind() const override;
    void unbind() const override;
    void create() override;
    void add_vertex_buffer(Buffer<T> *vbo) override;
    void set_index_buffer(Buffer<unsigned int> *ibo) override;
};
} // namespace mare

#endif