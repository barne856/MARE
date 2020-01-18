#ifndef CIRCLEMESH
#define CIRCLEMESH

#include "mare/Mesh.hpp"
#include "mare/Application.hpp"

#include "glm.hpp"

namespace mare
{
class CircleMesh : public Mesh
{
public:
    CircleMesh(int sides, float radius)
    {
        std::vector<float> verts;
        draw_method = DrawMethod::TRIANGLE_FAN;

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

        vertex_buffer = Application::GenFloatBuffer();
        vertex_buffer->create( verts );
        vertex_buffer->set_format({{ShaderDataType::Float2, "position"}});

        render_state = Application::GenRenderState();
        render_state->create();
        render_state->add_vertex_buffer(vertex_buffer);
    }

    ~CircleMesh()
    {
        delete vertex_buffer;
        delete render_state;
    }
};
} // namespace mare

#endif