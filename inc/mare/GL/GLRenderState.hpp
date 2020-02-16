#ifndef GLRENDERSTATE
#define GLRENDERSTATE

// MARE
#include "mare/RenderState.hpp"

// External Libraries
#include "glm.hpp"

// OpenGL
#include "GL/glew.h"

namespace mare
{
class GLRenderState : public RenderState
{
public:
    GLRenderState();
    ~GLRenderState();
    void bind() const override;
    void unbind() const override;
    void set_vertex_buffer(Buffer<float> *vbo) override;
    void set_index_buffer(Buffer<unsigned int> *ibo) override;
private:
GLenum gl_type(ShaderDataType type);
};
} // namespace mare

#endif