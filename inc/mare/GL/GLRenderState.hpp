#ifndef GLRENDERSTATE
#define GLRENDERSTATE

// MARE
#include "mare/RenderState.hpp"

// External Libraries
#include "glm.hpp"

namespace mare
{
class GLRenderState : public RenderState
{
public:
    GLRenderState() {}
    ~GLRenderState();
    void bind() const override;
    void unbind() const override;
    void create() override;
    void add_vertex_buffer(Buffer<float> *vbo) override;
    void set_index_buffer(Buffer<unsigned int> *ibo) override;
};
} // namespace mare

#endif