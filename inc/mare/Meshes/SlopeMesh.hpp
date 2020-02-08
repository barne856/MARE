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

        vertex_buffers = Renderer::API->GenFloatBuffer(1);
        vertex_buffers->create(&data[0][0], data.size() * sizeof(glm::vec3));
        vertex_buffers->set_format({{ShaderDataType::VEC3, "position"},
                                    {ShaderDataType::VEC3, "normal"}});

        render_state->create();
        render_state->add_vertex_buffer(vertex_buffers);
    }
};
} // namespace mare

#endif