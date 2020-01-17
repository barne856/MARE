#ifndef TRIANGLEMESH
#define TRIANGLEMESH

#include "mare/Mesh.hpp"
#include "mare/Application.hpp"

#include "glm.hpp"

namespace mare
{
class TriangleMesh : public Mesh
{
public:
    TriangleMesh(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
    {
        std::vector<float> verts;
        draw_method = DrawMethod::TRIANGLES;

        verts.push_back(v1[0]);
        verts.push_back(v1[1]);
        verts.push_back(v2[0]);
        verts.push_back(v2[1]);
        verts.push_back(v3[0]);
        verts.push_back(v3[1]);

        vertex_buffer = Application::GenFloatBuffer();
        vertex_buffer->create(verts);
        vertex_buffer->set_format({{ShaderDataType::Float2, "position"}});

        render_state = Application::GenRenderState();
        render_state->create();
        render_state->add_vertex_buffer(vertex_buffer);
    }

    ~TriangleMesh()
    {
        delete vertex_buffer;
        delete render_state;
    }
};
} // namespace mare

#endif