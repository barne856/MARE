#ifndef TRIANGLEMESH
#define TRIANGLEMESH

#include "mare/SimpleMesh.hpp"
#include "mare/Renderer.hpp"

#include "glm.hpp"

namespace mare
{
class TriangleMesh : public SimpleMesh
{
public:
    TriangleMesh(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
    {
        std::vector<float> verts;
        render_state->set_draw_method(DrawMethod::TRIANGLES);

        verts.push_back(v1[0]);
        verts.push_back(v1[1]);
        verts.push_back(v2[0]);
        verts.push_back(v2[1]);
        verts.push_back(v3[0]);
        verts.push_back(v3[1]);

        Scoped<Buffer<float>> vertex_buffer = Renderer::API->GenFloatBuffer(&verts);
        vertex_buffer->set_format({{ShaderDataType::VEC2, "position"}});

        render_state->set_vertex_buffer(std::move(vertex_buffer));
    }
};
} // namespace mare

#endif