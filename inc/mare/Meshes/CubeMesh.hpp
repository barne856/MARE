#ifndef CUBEMESH
#define CUBEMESH

#include "mare/SimpleMesh.hpp"
#include "mare/Renderer.hpp"

#include "glm.hpp"

namespace mare
{
class CubeMesh : public SimpleMesh
{
public:
    // scale: length of each face
    CubeMesh(float scale)
    {
        std::vector<glm::vec3> data;
        std::vector<unsigned int> indices;
        render_state->set_draw_method(DrawMethod::TRIANGLES);

        // front face vertices and normals, counterclockwise around normal from bottom left
        data.push_back({-0.5f * scale, -0.5f * scale, 0.5f * scale});
        data.push_back({0.0f, 0.0f, 1.0f});
        data.push_back({0.5f * scale, -0.5f * scale, 0.5f * scale});
        data.push_back({0.0f, 0.0f, 1.0f});
        data.push_back({0.5f * scale, 0.5f * scale, 0.5f * scale});
        data.push_back({0.0f, 0.0f, 1.0f});
        data.push_back({-0.5f * scale, 0.5f * scale, 0.5f * scale});
        data.push_back({0.0f, 0.0f, 1.0f});

        // right face vertices and normals, counterclockwise around normal from bottom left
        data.push_back({0.5f * scale, -0.5f * scale, 0.5f * scale});
        data.push_back({1.0f, 0.0f, 0.0f});
        data.push_back({0.5f * scale, -0.5f * scale, -0.5f * scale});
        data.push_back({1.0f, 0.0f, 0.0f});
        data.push_back({0.5f * scale, 0.5f * scale, -0.5f * scale});
        data.push_back({1.0f, 0.0f, 0.0f});
        data.push_back({0.5f * scale, 0.5f * scale, 0.5f * scale});
        data.push_back({1.0f, 0.0f, 0.0f});

        // back face vertices and normals, counterclockwise around normal from bottom left
        data.push_back({0.5f * scale, -0.5f * scale, -0.5f * scale});
        data.push_back({0.0f, 0.0f, -1.0f});
        data.push_back({-0.5f * scale, -0.5f * scale, -0.5f * scale});
        data.push_back({0.0f, 0.0f, -1.0f});
        data.push_back({-0.5f * scale, 0.5f * scale, -0.5f * scale});
        data.push_back({0.0f, 0.0f, -1.0f});
        data.push_back({0.5f * scale, 0.5f * scale, -0.5f * scale});
        data.push_back({0.0f, 0.0f, -1.0f});

        // left face vertices and normals, counterclockwise around normal from bottom left
        data.push_back({-0.5f * scale, -0.5f * scale, -0.5f * scale});
        data.push_back({-1.0f, 0.0f, 0.0f});
        data.push_back({-0.5f * scale, -0.5f * scale, 0.5f * scale});
        data.push_back({-1.0f, 0.0f, 0.0f});
        data.push_back({-0.5f * scale, 0.5f * scale, 0.5f * scale});
        data.push_back({-1.0f, 0.0f, 0.0f});
        data.push_back({-0.5f * scale, 0.5f * scale, -0.5f * scale});
        data.push_back({-1.0f, 0.0f, 0.0f});

        // top face vertices and normals, counterclockwise around normal from bottom left
        data.push_back({-0.5f * scale, 0.5f * scale, 0.5f * scale});
        data.push_back({0.0f, 1.0f, 0.0f});
        data.push_back({0.5f * scale, 0.5f * scale, 0.5f * scale});
        data.push_back({0.0f, 1.0f, 0.0f});
        data.push_back({0.5f * scale, 0.5f * scale, -0.5f * scale});
        data.push_back({0.0f, 1.0f, 0.0f});
        data.push_back({-0.5f * scale, 0.5f * scale, -0.5f * scale});
        data.push_back({0.0f, 1.0f, 0.0f});

        // bottom face vertices and normals, counterclockwise around normal from bottom left
        data.push_back({-0.5f * scale, -0.5f * scale, -0.5f * scale});
        data.push_back({0.0f, -1.0f, 0.0f});
        data.push_back({0.5f * scale, -0.5f * scale, -0.5f * scale});
        data.push_back({0.0f, -1.0f, 0.0f});
        data.push_back({0.5f * scale, -0.5f * scale, 0.5f * scale});
        data.push_back({0.0f, -1.0f, 0.0f});
        data.push_back({-0.5f * scale, -0.5f * scale, 0.5f * scale});
        data.push_back({0.0f, -1.0f, 0.0f});

        // front face indices
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(0);

        // right face indices
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(6);
        indices.push_back(6);
        indices.push_back(7);
        indices.push_back(4);

        // back face indices
        indices.push_back(8);
        indices.push_back(9);
        indices.push_back(10);
        indices.push_back(10);
        indices.push_back(11);
        indices.push_back(8);

        // left face indices
        indices.push_back(12);
        indices.push_back(13);
        indices.push_back(14);
        indices.push_back(14);
        indices.push_back(15);
        indices.push_back(12);

        // top face indices
        indices.push_back(16);
        indices.push_back(17);
        indices.push_back(18);
        indices.push_back(18);
        indices.push_back(19);
        indices.push_back(16);

        // bottom face indices
        indices.push_back(20);
        indices.push_back(21);
        indices.push_back(22);
        indices.push_back(22);
        indices.push_back(23);
        indices.push_back(20);

        vertex_buffers = Renderer::API->GenFloatBuffer(1);
        vertex_buffers->create(&data[0][0], data.size() * sizeof(glm::vec3));
        vertex_buffers->set_format({{ShaderDataType::VEC3, "position"},
                                   {ShaderDataType::VEC3, "normal"}});

        index_buffer = Renderer::API->GenIndexBuffer(1);
        index_buffer->create(indices);

        render_state->create();
        render_state->add_vertex_buffer(vertex_buffers);
        render_state->set_index_buffer(index_buffer);
    }
};
} // namespace mare

#endif