#ifndef CIRCLEMESH
#define CIRCLEMESH

#include "mare/SimpleMesh.hpp"
#include "mare/Renderer.hpp"

#include "glm.hpp"

namespace mare
{
class CircleMesh : public SimpleMesh
{
public:
    CircleMesh(int sides, float radius)
    {
        std::vector<float> verts;
        render_state->set_draw_method(DrawMethod::TRIANGLE_FAN);

        verts.push_back(0.0f);
        verts.push_back(0.0f);

        for (int i = 0; i < sides; i++)
        {
            float a = 2.0f * 3.141592653f * (float(i) / float(sides));
            verts.push_back(radius * cos(a));
            verts.push_back(radius * sin(a));
        }
        verts.push_back(radius);
        verts.push_back(0.0f);

        Scoped<Buffer<float>> vertex_buffer = Renderer::API->GenFloatBuffer(&verts);
        vertex_buffer->set_format({{ShaderDataType::VEC2, "position"}});

        render_state->set_vertex_buffer(std::move(vertex_buffer));
    }
};
} // namespace mare

#endif