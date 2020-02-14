#ifndef CYLINDERMESH
#define CYLINDERMESH

#include "mare/SimpleMesh.hpp"
#include "mare/Renderer.hpp"

#include "glm.hpp"

namespace mare
{
class CylinderMesh : public SimpleMesh
{
public:
    CylinderMesh(float start_angle, float end_angle, int sides)
    {
        std::vector<float> data;
        std::vector<unsigned int> indices;
        render_state->set_draw_method(DrawMethod::TRIANGLES);

        float theta = end_angle - start_angle;
        float dtheta = theta / sides;

        // outside edge
        float angle = start_angle;
        for (int i = 0; i < sides + 1; i++)
        {
            data.push_back((0.5f) * cos(angle));
            data.push_back((0.5f) * sin(angle));
            data.push_back(0.0f);
            data.push_back(cos(angle));
            data.push_back(sin(angle));
            data.push_back(0.0f);
            angle += dtheta;
        }

        angle = start_angle;
        for (int i = 0; i < sides + 1; i++)
        {
            data.push_back((0.5f) * cos(angle));
            data.push_back((0.5f) * sin(angle));
            data.push_back(1.0f);
            data.push_back(cos(angle));
            data.push_back(sin(angle));
            data.push_back(0.0f);
            angle += dtheta;
        }

        for (int i = 0; i < sides; i++)
        {
            indices.push_back(i + 1);
            indices.push_back(sides + i + 1);
            indices.push_back(i);
            indices.push_back(sides + i + 2);
            indices.push_back(sides + i + 1);
            indices.push_back(i + 1);
        }

        // bottom edge
        angle = start_angle;
        data.push_back(0.0f);
        data.push_back(0.0f);
        data.push_back(0.0f);
        data.push_back(0.0f);
        data.push_back(0.0f);
        data.push_back(-1.0f);
        for (int i = 0; i < sides + 1; i++)
        {
            data.push_back(0.5f * cos(-angle));
            data.push_back(0.5f * sin(-angle));
            data.push_back(0.0f);
            data.push_back(0.0f);
            data.push_back(0.0f);
            data.push_back(-1.0f);
            angle += dtheta;
        }

        for (int i = 0; i < sides + 1; i++)
        {
            indices.push_back(2 * (sides + 1));
            indices.push_back(2 * (sides + 1) + i + 1);
            indices.push_back(2 * (sides + 1) + i + 2);
        }

        // top edge
        angle = start_angle;
        data.push_back(0.0f);
        data.push_back(0.0f);
        data.push_back(1.0f);
        data.push_back(0.0f);
        data.push_back(0.0f);
        data.push_back(1.0f);
        for (int i = 0; i < sides + 1; i++)
        {
            data.push_back(0.5f * cos(angle));
            data.push_back(0.5f * sin(angle));
            data.push_back(1.0f);
            data.push_back(0.0f);
            data.push_back(0.0f);
            data.push_back(1.0f);
            angle += dtheta;
        }

        for (int i = 0; i < sides + 1; i++)
        {
            indices.push_back(3 * (sides + 1) + 1);
            indices.push_back(3 * (sides + 1) + i + 2);
            indices.push_back(3 * (sides + 1) + i + 3);
        }

        Buffer<float> *vertex_buffer = Renderer::API->GenFloatBuffer(&data);
        vertex_buffer->set_format({{LinalgDataType::VEC3, "position"},
                                   {LinalgDataType::VEC3, "normal"}});

        Buffer<unsigned int> *index_buffer = Renderer::API->GenIndexBuffer(&indices);

        render_state->set_vertex_buffer(vertex_buffer);
        render_state->set_index_buffer(index_buffer);
    }
};
} // namespace mare

#endif