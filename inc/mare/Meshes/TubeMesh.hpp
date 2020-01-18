#ifndef TUBEMESH
#define TUBEMESH

#include "mare/Mesh.hpp"
#include "mare/Application.hpp"

#include "glm.hpp"

namespace mare
{
class TubeMesh : public Mesh
{
public:
    // walls for hollow cylindrical structures
    // thickness is wall thickness / inner diameter = 1/(DR-2)
    // inner diameter is 1 with no scaling
    TubeMesh(float start_angle, float end_angle, float thickness, int sides)
    {
        const float PI = 3.141592653f;
        std::vector<float> data;
        std::vector<unsigned int> indes;
        draw_method = DrawMethod::TRIANGLES;

        float theta = end_angle - start_angle;
        float dtheta = theta / sides;

        // inside edge
        float angle = start_angle;
        for (int i = 0; i < sides + 1; i++)
        {
            data.push_back(0.5f * cos(angle));
            data.push_back(0.5f * sin(angle));
            data.push_back(0.0f);
            data.push_back(-cos(angle));
            data.push_back(-sin(angle));
            data.push_back(0.0f);
            angle += dtheta;
        }

        angle = start_angle;
        for (int i = 0; i < sides + 1; i++)
        {
            data.push_back(0.5f * cos(angle));
            data.push_back(0.5f * sin(angle));
            data.push_back(1.0f);
            data.push_back(-cos(angle));
            data.push_back(-sin(angle));
            data.push_back(0.0f);
            angle += dtheta;
        }

        for (int i = 0; i < sides; i++)
        {
            indes.push_back(i);
            indes.push_back(sides + i + 1);
            indes.push_back(i + 1);
            indes.push_back(i + 1);
            indes.push_back(sides + i + 1);
            indes.push_back(sides + i + 2);
        }

        // outside edge
        angle = start_angle;
        for (int i = 0; i < sides + 1; i++)
        {
            data.push_back((0.5f + thickness) * cos(angle));
            data.push_back((0.5f + thickness) * sin(angle));
            data.push_back(0.0f);
            data.push_back(cos(angle));
            data.push_back(sin(angle));
            data.push_back(0.0f);
            angle += dtheta;
        }

        angle = start_angle;
        for (int i = 0; i < sides + 1; i++)
        {
            data.push_back((0.5f + thickness) * cos(angle));
            data.push_back((0.5f + thickness) * sin(angle));
            data.push_back(1.0f);
            data.push_back(cos(angle));
            data.push_back(sin(angle));
            data.push_back(0.0f);
            angle += dtheta;
        }

        for (int i = 0; i < sides; i++)
        {
            indes.push_back(2 * (sides + 1) + i + 1);
            indes.push_back(3 * (sides + 1) + i);
            indes.push_back(2 * (sides + 1) + i);
            indes.push_back(3 * (sides + 1) + i + 1);
            indes.push_back(3 * (sides + 1) + i);
            indes.push_back(2 * (sides + 1) + i + 1);
        }

        // bottom edge
        angle = start_angle;
        for (int i = 0; i < sides + 1; i++)
        {
            data.push_back(0.5f * cos(angle));
            data.push_back(0.5f * sin(angle));
            data.push_back(0.0f);
            data.push_back(0.0f);
            data.push_back(0.0f);
            data.push_back(-1.0f);
            angle += dtheta;
        }

        angle = start_angle;
        for (int i = 0; i < sides + 1; i++)
        {
            data.push_back((0.5f + thickness) * cos(angle));
            data.push_back((0.5f + thickness) * sin(angle));
            data.push_back(0.0f);
            data.push_back(0.0f);
            data.push_back(0.0f);
            data.push_back(-1.0f);
            angle += dtheta;
        }

        for (int i = 0; i < sides; i++)
        {
            indes.push_back(4 * (sides + 1) + i + 1);
            indes.push_back(5 * (sides + 1) + i);
            indes.push_back(4 * (sides + 1) + i);
            indes.push_back(5 * (sides + 1) + i + 1);
            indes.push_back(5 * (sides + 1) + i);
            indes.push_back(4 * (sides + 1) + i + 1);
        }

        // top edge
        angle = start_angle;
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

        angle = start_angle;
        for (int i = 0; i < sides + 1; i++)
        {
            data.push_back((0.5f + thickness) * cos(angle));
            data.push_back((0.5f + thickness) * sin(angle));
            data.push_back(1.0f);
            data.push_back(0.0f);
            data.push_back(0.0f);
            data.push_back(1.0f);
            angle += dtheta;
        }

        for (int i = 0; i < sides; i++)
        {
            indes.push_back(6 * (sides + 1) + i);
            indes.push_back(6 * (sides + 1) + sides + i + 1);
            indes.push_back(6 * (sides + 1) + i + 1);
            indes.push_back(6 * (sides + 1) + i + 1);
            indes.push_back(6 * (sides + 1) + sides + i + 1);
            indes.push_back(6 * (sides + 1) + sides + i + 2);
        }

        // end edge
        data.push_back((0.5f) * cos(end_angle));
        data.push_back((0.5f) * sin(end_angle));
        data.push_back(0.0f);
        data.push_back(cos(end_angle + PI/2.0f));
        data.push_back(sin(end_angle + PI/2.0f));
        data.push_back(0.0f);
        data.push_back((0.5f) * cos(end_angle));
        data.push_back((0.5f) * sin(end_angle));
        data.push_back(1.0f);
        data.push_back(cos(end_angle + PI/2.0f));
        data.push_back(sin(end_angle + PI/2.0f));
        data.push_back(0.0f);
        data.push_back((0.5f + thickness) * cos(end_angle));
        data.push_back((0.5f + thickness) * sin(end_angle));
        data.push_back(0.0f);
        data.push_back(cos(end_angle + PI/2.0f));
        data.push_back(sin(end_angle + PI/2.0f));
        data.push_back(0.0f);
        data.push_back((0.5f + thickness) * cos(end_angle));
        data.push_back((0.5f + thickness) * sin(end_angle));
        data.push_back(1.0f);
        data.push_back(cos(end_angle + PI/2.0f));
        data.push_back(sin(end_angle + PI/2.0f));
        data.push_back(0.0f);
        indes.push_back(unsigned int(data.size() / 6 - 1));
        indes.push_back(unsigned int(data.size() / 6 - 2));
        indes.push_back(unsigned int(data.size() / 6 - 3));
        indes.push_back(unsigned int(data.size() / 6 - 4));
        indes.push_back(unsigned int(data.size() / 6 - 3));
        indes.push_back(unsigned int(data.size() / 6 - 2));

        // start edge
        data.push_back((0.5f) * cos(start_angle));
        data.push_back((0.5f) * sin(start_angle));
        data.push_back(0.0f);
        data.push_back(cos(start_angle - PI/2.0f));
        data.push_back(sin(start_angle - PI/2.0f));
        data.push_back(0.0f);
        data.push_back((0.5f) * cos(start_angle));
        data.push_back((0.5f) * sin(start_angle));
        data.push_back(1.0f);
        data.push_back(cos(start_angle - PI/2.0f));
        data.push_back(sin(start_angle - PI/2.0f));
        data.push_back(0.0f);
        data.push_back((0.5f + thickness) * cos(start_angle));
        data.push_back((0.5f + thickness) * sin(start_angle));
        data.push_back(0.0f);
        data.push_back(cos(start_angle - PI/2.0f));
        data.push_back(sin(start_angle - PI/2.0f));
        data.push_back(0.0f);
        data.push_back((0.5f + thickness) * cos(start_angle));
        data.push_back((0.5f + thickness) * sin(start_angle));
        data.push_back(1.0f);
        data.push_back(cos(start_angle - PI/2.0f));
        data.push_back(sin(start_angle - PI/2.0f));
        data.push_back(0.0f);
        indes.push_back(unsigned int(data.size() / 6 - 3));
        indes.push_back(unsigned int(data.size() / 6 - 2));
        indes.push_back(unsigned int(data.size() / 6 - 1));
        indes.push_back(unsigned int(data.size() / 6 - 2));
        indes.push_back(unsigned int(data.size() / 6 - 3));
        indes.push_back(unsigned int(data.size() / 6 - 4));

        vertex_buffer = Application::GenFloatBuffer();
        vertex_buffer->create(data);
        vertex_buffer->set_format({{ShaderDataType::Float3, "position"},
                                   {ShaderDataType::Float3, "normal"}});

        index_buffer = Application::GenIndexBuffer();
        index_buffer->create(indes);

        render_state = Application::GenRenderState();
        render_state->create();
        render_state->add_vertex_buffer(vertex_buffer);
        render_state->set_index_buffer(index_buffer);
    }

    ~TubeMesh()
    {
        delete vertex_buffer;
        delete index_buffer;
        delete render_state;
    }
};
} // namespace mare

#endif