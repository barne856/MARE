#ifndef TUBEMESH
#define TUBEMESH

// MARE
#include "Meshes.hpp"
#include "Renderer.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {
/**
 * @brief A SimpleMesh of a 3D tube. Like a hollow cylinder or pipe.
 */
class TubeMesh : public SimpleMesh {
public:
  /**
   * @brief Construct a new TubeMesh.
   * @details The thickness is the wall thickness / inner diameter = 1/(DR-2)
   * and the inner diameter is 1 in model space.
   * @param start_angle The starting angle of the tube in radians.
   * @param end_angle The ending angle of the tube in radians.
   * @param thickness The thickness of the tube.
   * @param sides The number of sides used to approximate the tube.
   */
  TubeMesh(float start_angle, float end_angle, float thickness, int sides) {
    const float PI = 3.141592653f;
    std::vector<float> data;
    std::vector<unsigned int> indes;
    set_draw_method(DrawMethod::TRIANGLES);

    float theta = end_angle - start_angle;
    float dtheta = theta / sides;

    // inside edge
    float angle = start_angle;
    for (int i = 0; i < sides + 1; i++) {
      data.push_back(0.5f * cos(angle));
      data.push_back(0.5f * sin(angle));
      data.push_back(0.0f);
      data.push_back(-cos(angle));
      data.push_back(-sin(angle));
      data.push_back(0.0f);
      angle += dtheta;
    }

    angle = start_angle;
    for (int i = 0; i < sides + 1; i++) {
      data.push_back(0.5f * cos(angle));
      data.push_back(0.5f * sin(angle));
      data.push_back(1.0f);
      data.push_back(-cos(angle));
      data.push_back(-sin(angle));
      data.push_back(0.0f);
      angle += dtheta;
    }

    for (int i = 0; i < sides; i++) {
      indes.push_back(i);
      indes.push_back(sides + i + 1);
      indes.push_back(i + 1);
      indes.push_back(i + 1);
      indes.push_back(sides + i + 1);
      indes.push_back(sides + i + 2);
    }

    // outside edge
    angle = start_angle;
    for (int i = 0; i < sides + 1; i++) {
      data.push_back((0.5f + thickness) * cos(angle));
      data.push_back((0.5f + thickness) * sin(angle));
      data.push_back(0.0f);
      data.push_back(cos(angle));
      data.push_back(sin(angle));
      data.push_back(0.0f);
      angle += dtheta;
    }

    angle = start_angle;
    for (int i = 0; i < sides + 1; i++) {
      data.push_back((0.5f + thickness) * cos(angle));
      data.push_back((0.5f + thickness) * sin(angle));
      data.push_back(1.0f);
      data.push_back(cos(angle));
      data.push_back(sin(angle));
      data.push_back(0.0f);
      angle += dtheta;
    }

    for (int i = 0; i < sides; i++) {
      indes.push_back(2 * (sides + 1) + i + 1);
      indes.push_back(3 * (sides + 1) + i);
      indes.push_back(2 * (sides + 1) + i);
      indes.push_back(3 * (sides + 1) + i + 1);
      indes.push_back(3 * (sides + 1) + i);
      indes.push_back(2 * (sides + 1) + i + 1);
    }

    // bottom edge
    angle = start_angle;
    for (int i = 0; i < sides + 1; i++) {
      data.push_back(0.5f * cos(angle));
      data.push_back(0.5f * sin(angle));
      data.push_back(0.0f);
      data.push_back(0.0f);
      data.push_back(0.0f);
      data.push_back(-1.0f);
      angle += dtheta;
    }

    angle = start_angle;
    for (int i = 0; i < sides + 1; i++) {
      data.push_back((0.5f + thickness) * cos(angle));
      data.push_back((0.5f + thickness) * sin(angle));
      data.push_back(0.0f);
      data.push_back(0.0f);
      data.push_back(0.0f);
      data.push_back(-1.0f);
      angle += dtheta;
    }

    for (int i = 0; i < sides; i++) {
      indes.push_back(4 * (sides + 1) + i + 1);
      indes.push_back(5 * (sides + 1) + i);
      indes.push_back(4 * (sides + 1) + i);
      indes.push_back(5 * (sides + 1) + i + 1);
      indes.push_back(5 * (sides + 1) + i);
      indes.push_back(4 * (sides + 1) + i + 1);
    }

    // top edge
    angle = start_angle;
    for (int i = 0; i < sides + 1; i++) {
      data.push_back(0.5f * cos(angle));
      data.push_back(0.5f * sin(angle));
      data.push_back(1.0f);
      data.push_back(0.0f);
      data.push_back(0.0f);
      data.push_back(1.0f);
      angle += dtheta;
    }

    angle = start_angle;
    for (int i = 0; i < sides + 1; i++) {
      data.push_back((0.5f + thickness) * cos(angle));
      data.push_back((0.5f + thickness) * sin(angle));
      data.push_back(1.0f);
      data.push_back(0.0f);
      data.push_back(0.0f);
      data.push_back(1.0f);
      angle += dtheta;
    }

    for (int i = 0; i < sides; i++) {
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
    data.push_back(cos(end_angle + PI / 2.0f));
    data.push_back(sin(end_angle + PI / 2.0f));
    data.push_back(0.0f);
    data.push_back((0.5f) * cos(end_angle));
    data.push_back((0.5f) * sin(end_angle));
    data.push_back(1.0f);
    data.push_back(cos(end_angle + PI / 2.0f));
    data.push_back(sin(end_angle + PI / 2.0f));
    data.push_back(0.0f);
    data.push_back((0.5f + thickness) * cos(end_angle));
    data.push_back((0.5f + thickness) * sin(end_angle));
    data.push_back(0.0f);
    data.push_back(cos(end_angle + PI / 2.0f));
    data.push_back(sin(end_angle + PI / 2.0f));
    data.push_back(0.0f);
    data.push_back((0.5f + thickness) * cos(end_angle));
    data.push_back((0.5f + thickness) * sin(end_angle));
    data.push_back(1.0f);
    data.push_back(cos(end_angle + PI / 2.0f));
    data.push_back(sin(end_angle + PI / 2.0f));
    data.push_back(0.0f);
    indes.push_back(static_cast<unsigned int>(data.size() / 6 - 1));
    indes.push_back(static_cast<unsigned int>(data.size() / 6 - 2));
    indes.push_back(static_cast<unsigned int>(data.size() / 6 - 3));
    indes.push_back(static_cast<unsigned int>(data.size() / 6 - 4));
    indes.push_back(static_cast<unsigned int>(data.size() / 6 - 3));
    indes.push_back(static_cast<unsigned int>(data.size() / 6 - 2));

    // start edge
    data.push_back((0.5f) * cos(start_angle));
    data.push_back((0.5f) * sin(start_angle));
    data.push_back(0.0f);
    data.push_back(cos(start_angle - PI / 2.0f));
    data.push_back(sin(start_angle - PI / 2.0f));
    data.push_back(0.0f);
    data.push_back((0.5f) * cos(start_angle));
    data.push_back((0.5f) * sin(start_angle));
    data.push_back(1.0f);
    data.push_back(cos(start_angle - PI / 2.0f));
    data.push_back(sin(start_angle - PI / 2.0f));
    data.push_back(0.0f);
    data.push_back((0.5f + thickness) * cos(start_angle));
    data.push_back((0.5f + thickness) * sin(start_angle));
    data.push_back(0.0f);
    data.push_back(cos(start_angle - PI / 2.0f));
    data.push_back(sin(start_angle - PI / 2.0f));
    data.push_back(0.0f);
    data.push_back((0.5f + thickness) * cos(start_angle));
    data.push_back((0.5f + thickness) * sin(start_angle));
    data.push_back(1.0f);
    data.push_back(cos(start_angle - PI / 2.0f));
    data.push_back(sin(start_angle - PI / 2.0f));
    data.push_back(0.0f);
    indes.push_back(static_cast<unsigned int>(data.size() / 6 - 3));
    indes.push_back(static_cast<unsigned int>(data.size() / 6 - 2));
    indes.push_back(static_cast<unsigned int>(data.size() / 6 - 1));
    indes.push_back(static_cast<unsigned int>(data.size() / 6 - 2));
    indes.push_back(static_cast<unsigned int>(data.size() / 6 - 3));
    indes.push_back(static_cast<unsigned int>(data.size() / 6 - 4));

    Scoped<Buffer<float>> vertex_buffer =
        Renderer::gen_buffer<float>(&data[0], data.size() * sizeof(float));
    vertex_buffer->set_format({{AttributeType::POSITON_3D, "position"},
                               {AttributeType::NORMAL, "normal"}});

    Scoped<Buffer<unsigned int>> index_buffer = Renderer::gen_buffer<uint32_t>(
        &indes[0], indes.size() * sizeof(uint32_t));

    add_geometry_buffer(std::move(vertex_buffer));
    set_index_buffer(std::move(index_buffer));
  }
};
} // namespace mare

#endif