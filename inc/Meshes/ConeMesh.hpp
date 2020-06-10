#ifndef CONEMESH
#define CONEMESH

// MARE
#include "Meshes.hpp"
#include "Renderer.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {
/**
 * @brief A SimpleMesh of a 3D Cone.
 */
class ConeMesh : public SimpleMesh {
public:
  // height is always equal to the sqrt(3)*radius
  /**
   * @brief Construct a new ConeMesh.
   * @details The Cone height is always equal to sqrt(3)*radius in model space.
   * Different heights can be achieved by scaling the model in the z direction.
   *
   * @param radius The radius of the ConeMesh in model space.
   * @param sides The sides used to approximate the ConeMesh.
   */
  ConeMesh(float radius, unsigned int sides) {
    std::vector<glm::vec3> verts{};
    std::vector<glm::vec3> normals{};
    std::vector<unsigned int> indices;
    set_draw_method(DrawMethod::TRIANGLES);

    const float PI = 3.141592653f;
    const float SQRT3 = 1.73205080757f;
    const float SQRT2 = 1.41421356237f;
    float angle = 0;
    float delta_angle = (2.0f * PI) / float(sides);
    for (unsigned int i = 0; i < sides; i++) {
      glm::vec3 normal =
          glm::vec3(SQRT2 * cos(angle + 0.5f * delta_angle),
                    SQRT2 * sin(angle + 0.5f * delta_angle), 1.0f) /
          SQRT3;
      verts.push_back({radius * cos(angle), radius * sin(angle), 0.0f});
      normals.push_back(normal);
      verts.push_back({radius * cos(angle + delta_angle),
                       radius * sin(angle + delta_angle), 0.0f});
      normals.push_back(normal);
      verts.push_back({0.0f, 0.0f, radius * SQRT2});
      normals.push_back(normal);
      angle += delta_angle;
    }
    const glm::vec3 down_normal = glm::vec3(0.0f, 0.0f, -1.0f);
    angle -= delta_angle;
    for (unsigned int i = 0; i < sides - 2; i++) {
      verts.push_back({radius, 0.0f, 0.0f});
      normals.push_back(down_normal);
      verts.push_back({radius * cos(angle), radius * sin(angle), 0.0f});
      normals.push_back(down_normal);
      verts.push_back({radius * cos(angle - delta_angle),
                       radius * sin(angle - delta_angle), 0.0f});
      normals.push_back(down_normal);
      angle -= delta_angle;
    }

    // interleave vertex positions and normals
    std::vector<float> vertex_data{};
    for (size_t i = 0; i < verts.size(); i++) {
      vertex_data.push_back(verts[i][0]);
      vertex_data.push_back(verts[i][1]);
      vertex_data.push_back(verts[i][2]);
      vertex_data.push_back(normals[i][0]);
      vertex_data.push_back(normals[i][1]);
      vertex_data.push_back(normals[i][2]);
    }

    Scoped<Buffer<float>> vb = Renderer::gen_buffer<float>(
        &vertex_data[0], vertex_data.size() * sizeof(float));
    vb->set_format({{AttributeType::POSITON_3D, "position"},
                    {AttributeType::NORMAL, "normal"}});

    add_geometry_buffer(std::move(vb));
  }
};
} // namespace mare

#endif