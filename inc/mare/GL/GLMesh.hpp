#ifndef GLMESH
#define GLMESH

// MARE
#include "mare/Mesh.hpp"
#include "mare/GL/GLRenderer.hpp"
#include "mare/GL/VertexArray.hpp"

namespace mare
{

class GLMesh : virtual public Mesh, protected GLRenderer
{
public:
    virtual ~GLMesh() {}
    void render(Material* material) override;
    void render(Material *material, glm::mat4 model) override;
    void bind() const { m_vao->bind(); }

protected:
    VertexArray *m_vao = nullptr;
    GLBuffer<float> *m_vbo = nullptr;
    GLBuffer<unsigned int> *m_ibo = nullptr;
    GLenum m_draw_method;
};

} // namespace mare

#endif