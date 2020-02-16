#ifndef SLOPEMESH
#define SLOPEMESH

#include "mare/SimpleMesh.hpp"
#include "mare/Renderer.hpp"

#include "glm.hpp"

namespace mare
{
class SlopeMesh : public SimpleMesh
{
public:
    // scale is the length of the side of the bounding cube
    SlopeMesh(float scale)
    {
        std::vector<glm::vec3> data;
        render_state->set_draw_method(DrawMethod::TRIANGLES);
        const float SQRT2 = 1.41421356237f;

        // front face
        data.push_back({-0.5f * scale, -0.5f * scale, 0.5f * scale});
        data.push_back({0.0f, 0.0f, 1.0f});
        data.push_back({0.5f * scale, -0.5f * scale, 0.5f * scale});
        data.push_back({0.0f, 0.0f, 1.0f});
        data.push_back({0.5f * scale, 0.5f * scale, 0.5f * scale});
        data.push_back({0.0f, 0.0f, 1.0f});

        // back face
        data.push_back({0.5f * scale, -0.5f * scale, -0.5f * scale});
        data.push_back({0.0f, 0.0f, -1.0f});
        data.push_back({-0.5f * scale, -0.5f * scale, -0.5f * scale});
        data.push_back({0.0f, 0.0f, -1.0f});
        data.push_back({0.5f * scale, 0.5f * scale, -0.5f * scale});
        data.push_back({0.0f, 0.0f, -1.0f});

        // right face
        data.push_back({0.5f * scale, -0.5f * scale, 0.5f * scale});
        data.push_back({1.0f, 0.0f, 0.0f});
        data.push_back({0.5f * scale, -0.5f * scale, -0.5f * scale});
        data.push_back({1.0f, 0.0f, 0.0f});
        data.push_back({0.5f * scale, 0.5f * scale, -0.5f * scale});
        data.push_back({1.0f, 0.0f, 0.0f});
        data.push_back({0.5f * scale, -0.5f * scale, 0.5f * scale});
        data.push_back({1.0f, 0.0f, 0.0f});
        data.push_back({0.5f * scale, 0.5f * scale, -0.5f * scale});
        data.push_back({1.0f, 0.0f, 0.0f});
        data.push_back({0.5f * scale, 0.5f * scale, 0.5f * scale});
        data.push_back({1.0f, 0.0f, 0.0f});

        // bottom face
        data.push_back({0.5f * scale, -0.5f * scale, 0.5f * scale});
        data.push_back({0.0f, -1.0f, 0.0f});
        data.push_back({-0.5f * scale, -0.5f * scale, -0.5f * scale});
        data.push_back({0.0f, -1.0f, 0.0f});
        data.push_back({0.5f * scale, -0.5f * scale, -0.5f * scale});
        data.push_back({0.0f, -1.0f, 0.0f});
        data.push_back({0.5f * scale, -0.5f * scale, 0.5f * scale});
        data.push_back({0.0f, -1.0f, 0.0f});
        data.push_back({-0.5f * scale, -0.5f * scale, 0.5f * scale});
        data.push_back({0.0f, -1.0f, 0.0f});
        data.push_back({-0.5f * scale, -0.5f * scale, -0.5f * scale});
        data.push_back({0.0f, -1.0f, 0.0f});

        // slope face
        data.push_back({-0.5f * scale, -0.5f * scale, 0.5f * scale});
        data.push_back({-SQRT2 / 2.0f, SQRT2 / 2.0f, 0.0f});
        data.push_back({0.5f * scale, 0.5f * scale, 0.5f * scale});
        data.push_back({-SQRT2 / 2.0f, SQRT2 / 2.0f, 0.0f});
        data.push_back({0.5f * scale, 0.5f * scale, -0.5f * scale});
        data.push_back({-SQRT2 / 2.0f, SQRT2 / 2.0f, 0.0f});
        data.push_back({-0.5f * scale, -0.5f * scale, 0.5f * scale});
        data.push_back({-SQRT2 / 2.0f, SQRT2 / 2.0f, 0.0f});
        data.push_back({0.5f * scale, 0.5f * scale, -0.5f * scale});
        data.push_back({-SQRT2 / 2.0f, SQRT2 / 2.0f, 0.0f});
        data.push_back({-0.5f * scale, -0.5f * scale, -0.5f * scale});
        data.push_back({-SQRT2 / 2.0f, SQRT2 / 2.0f, 0.0f});

        std::vector<float> vertex_data{};
        for(auto& vert : data)
        {
            vertex_data.push_back(vert[0]);
            vertex_data.push_back(vert[1]);
            vertex_data.push_back(vert[2]);
        }

        Buffer<float> *vertex_buffer = Renderer::API->GenFloatBuffer(&vertex_data);
        vertex_buffer->set_format({{ShaderDataType::VEC3, "position"},
                                    {ShaderDataType::VEC3, "normal"}});

        render_state->set_vertex_buffer(vertex_buffer);
    }
};
} // namespace mare

#endif