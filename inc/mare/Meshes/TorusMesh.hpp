#ifndef TORUSMESH
#define TORUSMESH

#include "mare/SimpleMesh.hpp"
#include "mare/Application.hpp"

#include "glm.hpp"

namespace mare
{
class TorusMesh : public SimpleMesh<glm::vec3>
{
public:
    TorusMesh(unsigned int n_segments, unsigned int n_rings, float inner_radius, float outer_radius)
        : r(outer_radius), R(inner_radius), q(n_segments), p(n_rings)
    {
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec3> texcoords;
        std::vector<unsigned int> indices;
        render_state->set_draw_method(DrawMethod::TRIANGLE_STRIP);

        // Compute torus vertices and normals
        for (unsigned int i = 0; i <= p; ++i)
        {
            for (unsigned int j = 0; j <= q; ++j)
            {
                // Compute texture coordinates (surface parameters)
                // Note that u is added to j. This creates a spiral pattern, which
                // allows us to draw the entire torus in one triangle strip without
                // using degenerate triangles.
                // (Yet we still need degenerate triangles to avoid texture
                // wrap-around)
                float u = float(i) / p;
                float v = (float(j) + u) / q;

                // Compute angles
                float u_angle = u * 2 * PI;
                float v_angle = v * 2 * PI;

                // Position
                float x = cos(u_angle) * (r + cos(v_angle) * R);
                float y = sin(u_angle) * (r + cos(v_angle) * R);
                float z = sin(v_angle) * R;

                // Normal vector
                float nx = cos(u_angle) * cos(v_angle);
                float ny = sin(u_angle) * cos(v_angle);
                float nz = sin(v_angle);

                vertices.push_back(glm::vec3(x, y, z));
                normals.push_back(glm::vec3(nx, ny, nz));
                texcoords.push_back(glm::vec3(u, v, 0.0f));
            }
        }

        // Compute torus indices
        unsigned int n_vertices = static_cast<unsigned int>(vertices.size());
        for (unsigned int i = 0; i <= n_vertices; ++i)
        {
            indices.push_back(static_cast<unsigned int>(i % n_vertices));
            indices.push_back(static_cast<unsigned int>((i + q + 1) % n_vertices));
        }

        vertex_buffers = Application::GenBuffer<glm::vec3>(3);
        vertex_buffers[0].create(vertices);
        vertex_buffers[0].set_format({{ShaderDataType::VEC3, "position"}});
        vertex_buffers[1].create(normals);
        vertex_buffers[1].set_format({{ShaderDataType::VEC3, "normal"}});
        vertex_buffers[2].create(texcoords);
        vertex_buffers[2].set_format({{ShaderDataType::VEC3, "texcoords"}});

        index_buffer = Application::GenBuffer<unsigned int>(1);
        index_buffer->create(indices);

        render_state->create();
        render_state->add_vertex_buffer(&vertex_buffers[0]);
        render_state->add_vertex_buffer(&vertex_buffers[1]);
        render_state->add_vertex_buffer(&vertex_buffers[2]);
        render_state->set_index_buffer(index_buffer);
    }

private:
    float r;        // outer radius
    float R;        // inner radius
    unsigned int p; // number of mesh columns
    unsigned int q; // number of mesh row
    const float PI = 3.141592653f;

    // Fuctions to map the grid vertex (u_i,v_j) to the mesh vertex (f(u_i,v_j), g(u_i,v_j), h(u_i,v_j)) on the torus.
    float f(int i, int j)
    {
        return ((R + r * cos((-1 + 2 * (float)j / q) * PI)) * cos((-1 + 2 * (float)i / p) * PI));
    }

    float g(int i, int j)
    {
        return ((R + r * cos((-1 + 2 * (float)j / q) * PI)) * sin((-1 + 2 * (float)i / p) * PI));
    }

    float h(int i, int j)
    {
        return (r * sin((-1 + 2 * (float)j / q) * PI));
    }
};
} // namespace mare

#endif