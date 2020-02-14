#ifndef QUADRANGLEMESH
#define QUADRANGLEMESH

#include "mare/SimpleMesh.hpp"
#include "mare/Renderer.hpp"

#include "glm.hpp"

namespace mare
{
class QuadrangleMesh : public SimpleMesh
{
public:
    QuadrangleMesh()
    {
        std::vector<float> verts;
        render_state->set_draw_method(DrawMethod::TRIANGLES);

        verts.push_back(-0.5f);
        verts.push_back(-0.5f);
        verts.push_back(0.5f);
        verts.push_back(-0.5f);
        verts.push_back(0.5f);
        verts.push_back(0.5f);
        verts.push_back(0.5f);
        verts.push_back(0.5f);
        verts.push_back(-0.5f);
        verts.push_back(0.5f);
        verts.push_back(-0.5f);
        verts.push_back(-0.5f);

        Buffer<float> *vertex_buffer = Renderer::API->GenFloatBuffer(&verts);
        vertex_buffer->set_format({{LinalgDataType::VEC2, "position"}});

        render_state->set_vertex_buffer(vertex_buffer);
    }
    QuadrangleMesh(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec2 v4)
    {
        std::vector<float> verts;
        render_state->set_draw_method(DrawMethod::TRIANGLES);

        verts.push_back(v1[0]);
        verts.push_back(v1[1]);
        verts.push_back(v2[0]);
        verts.push_back(v2[1]);
        verts.push_back(v3[0]);
        verts.push_back(v3[1]);
        verts.push_back(v3[0]);
        verts.push_back(v3[1]);
        verts.push_back(v4[0]);
        verts.push_back(v4[1]);
        verts.push_back(v1[0]);
        verts.push_back(v1[1]);

        Buffer<float> *vertex_buffer = Renderer::API->GenFloatBuffer(&verts);
        vertex_buffer->set_format({{LinalgDataType::VEC2, "position"}});

        render_state->set_vertex_buffer(vertex_buffer);
    }
};
} // namespace mare

#endif