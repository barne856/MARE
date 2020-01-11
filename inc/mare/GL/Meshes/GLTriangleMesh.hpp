#ifndef GLTRIANGLEMESH
#define GLTRIANGLEMESH

#include "mare/GL/GLMesh.hpp"

#include "glm.hpp"

namespace mare
{
class GLTriangleMesh : public GLMesh
{
public:
    GLTriangleMesh(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
    {
        std::vector<float> verts;
        m_draw_method = GL_TRIANGLES;

        verts.push_back(v1[0]);
        verts.push_back(v1[1]);
        verts.push_back(v2[0]);
        verts.push_back(v2[1]);
        verts.push_back(v3[0]);
        verts.push_back(v3[1]);

        m_vbo = new GLBuffer<float>();
        m_vbo->create(verts);
        m_vbo->set_format({{ShaderDataType::Float2, "position"}});

        m_vao = new VertexArray();
        m_vao->create();
        m_vao->add_vertex_buffer(m_vbo);
    }

    ~GLTriangleMesh()
    {
        delete m_vbo;
        delete m_vao;
    }
};
} // namespace mare

#endif