#ifndef CIRCLEMESH
#define CIRCLEMESH

#include "mare/SimpleMesh.hpp"
#include "mare/Application.hpp"

#include "glm.hpp"

namespace mare
{
class CircleMesh : public SimpleMesh<float>
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

        vertex_buffers = Application::GenBuffer<float>(1);
        vertex_buffers->create(verts);
        vertex_buffers->set_format({{ShaderDataType::VEC2, "position"}});

        render_state->create();
        render_state->add_vertex_buffer(vertex_buffers);
    }
};
} // namespace mare

#endif