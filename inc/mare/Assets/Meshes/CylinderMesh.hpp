#ifndef CYLINDERMESH
#define CYLINDERMESH

// MARE
#include "mare/Meshes.hpp"
#include "mare/Renderer.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {
/**
 * @brief A SimpleMesh of a 3D Cylinder.
 * @details Contains vertices and normals.
 */
class CylinderMesh : public SimpleMesh {
public:
  /**
   * @brief Construct a new CylinderMesh.
   *
   * @param start_angle The starting angle of the Cylinder in radians.
   * @param end_angle The ending angle of the Cylinder in radians.
   * @param sides The sides used to approximate the Cylinder.
   */
  CylinderMesh(float start_angle, float end_angle, int sides) {
    std::vector<float> data;
    std::vector<unsigned int> indices;
    set_draw_method(DrawMethod::TRIANGLES);

    float theta = end_angle - start_angle;
    float dtheta = theta / sides;

    // outside edge
    float angle = start_angle;
    for (int i = 0; i < sides + 1; i++) {
      data.push_back((0.5f) * cos(angle));
      data.push_back((0.5f) * sin(angle));
      data.push_back(0.0f);
      data.push_back(cos(angle));
      data.push_back(sin(angle));
      data.push_back(0.0f);
      angle += dtheta;
    }

    angle = start_angle;
    for (int i = 0; i < sides + 1; i++) {
      data.push_back((0.5f) * cos(angle));
      data.push_back((0.5f) * sin(angle));
      data.push_back(1.0f);
      data.push_back(cos(angle));
      data.push_back(sin(angle));
      data.push_back(0.0f);
      angle += dtheta;
    }

    for (int i = 0; i < sides; i++) {
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
    for (int i = 0; i < sides + 1; i++) {
      data.push_back(0.5f * cos(-angle));
      data.push_back(0.5f * sin(-angle));
      data.push_back(0.0f);
      data.push_back(0.0f);
      data.push_back(0.0f);
      data.push_back(-1.0f);
      angle += dtheta;
    }

    for (int i = 0; i < sides + 1; i++) {
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
    for (int i = 0; i < sides + 1; i++) {
      data.push_back(0.5f * cos(angle));
      data.push_back(0.5f * sin(angle));
      data.push_back(1.0f);
      data.push_back(0.0f);
      data.push_back(0.0f);
      data.push_back(1.0f);
      angle += dtheta;
    }

    for (int i = 0; i < sides + 1; i++) {
      indices.push_back(3 * (sides + 1) + 1);
      indices.push_back(3 * (sides + 1) + i + 2);
      indices.push_back(3 * (sides + 1) + i + 3);
    }

    Scoped<Buffer<float>> vertex_buffer =
        Renderer::gen_buffer<float>(&data[0], data.size() * sizeof(float));
    vertex_buffer->set_format({{AttributeType::POSITON_3D, "position"},
                               {AttributeType::NORMAL, "normal"}});

    Scoped<Buffer<unsigned int>> index_buffer = Renderer::gen_buffer<uint32_t>(
        &indices[0], indices.size() * sizeof(uint32_t));

    add_geometry_buffer(std::move(vertex_buffer));
    set_index_buffer(std::move(index_buffer));
  }
};
} // namespace mare

#endif