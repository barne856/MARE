#ifndef CONEMESH
#define CONEMESH

#include "mare/SimpleMesh.hpp"
#include "mare/Application.hpp"

#include "glm.hpp"

namespace mare
{
class ConeMesh : public SimpleMesh<glm::vec3>
{
public:
    // height is always equal to the sqrt(3)*radius
    ConeMesh(float radius, unsigned int sides)
    {
        std::vector<glm::vec3> verts{};
        std::vector<glm::vec3> normals{};
        std::vector<unsigned int> indices;
        render_state->set_draw_method(DrawMethod::TRIANGLES);

        const float PI = 3.141592653f;
        const float SQRT3 = 1.73205080757f;
        const float SQRT2 = 1.41421356237f;
        float angle = 0;
        float delta_angle = (2.0f * PI) / float(sides);
        for (unsigned int i = 0; i < sides; i++)
        {
            glm::vec3 normal = glm::vec3(SQRT2 * cos(angle + 0.5f * delta_angle), SQRT2 * sin(angle + 0.5f * delta_angle), 1.0f) / SQRT3;
            verts.push_back({radius * cos(angle), radius * sin(angle), 0.0f});
            normals.push_back(normal);
            verts.push_back({radius * cos(angle + delta_angle), radius * sin(angle + delta_angle), 0.0f});
            normals.push_back(normal);
            verts.push_back({0.0f, 0.0f, radius * SQRT2});
            normals.push_back(normal);
            angle += delta_angle;
        }
        const glm::vec3 down_normal = glm::vec3(0.0f, 0.0f, -1.0f);
        angle -= delta_angle;
        for (unsigned int i = 0; i < sides - 2; i++)
        {
            verts.push_back({radius, 0.0f, 0.0f});
            normals.push_back(down_normal);
            verts.push_back({radius * cos(angle), radius * sin(angle), 0.0f});
            normals.push_back(down_normal);
            verts.push_back({radius * cos(angle - delta_angle), radius * sin(angle - delta_angle), 0.0f});
            normals.push_back(down_normal);
            angle -= delta_angle;
        }

        vertex_buffers = Application::GenBuffer<glm::vec3>(2);
        vertex_buffers[0].create(verts);
        vertex_buffers[0].set_format({{ShaderDataType::VEC3, "position"}});
        vertex_buffers[1].create(normals);
        vertex_buffers[1].set_format({{ShaderDataType::VEC3, "normal"}});

        render_state->create();
        render_state->add_vertex_buffer(&vertex_buffers[0]);
        render_state->add_vertex_buffer(&vertex_buffers[1]);
    }
};
} // namespace mare

#endif