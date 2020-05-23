#ifndef SLOPEMESH
#define SLOPEMESH

// MARE
#include "mare/Meshes.hpp"
#include "mare/Renderer.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {
/**
 * @brief A SimpleMesh of a 3D sloped block with a 45 degree incline and equal
 * width, height, and depth in model space.
 */
class SlopeMesh : public SimpleMesh {
public:
  /**
   * @brief Construct a new SlopeMesh.
   *
   * @param scale The length of the sides of the bounding cube.
   */
  SlopeMesh(float scale) {
    std::vector<glm::vec3> data;
    set_draw_method(DrawMethod::TRIANGLES);
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
    for (auto &vert : data) {
      vertex_data.push_back(vert[0]);
      vertex_data.push_back(vert[1]);
      vertex_data.push_back(vert[2]);
    }

    Scoped<Buffer<float>> vertex_buffer = Renderer::gen_buffer<float>(
        &vertex_data[0], vertex_data.size() * sizeof(float));
    vertex_buffer->set_format({{AttributeType::POSITON_3D, "position"},
                               {AttributeType::NORMAL, "normal"}});

    add_geometry_buffer(std::move(vertex_buffer));
  }
};
} // namespace mare

#endif