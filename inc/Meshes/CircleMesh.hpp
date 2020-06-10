#ifndef CIRCLEMESH
#define CIRCLEMESH

// MARE
#include "Meshes.hpp"
#include "Renderer.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {
/**
 * @brief A SimpleMesh of a 2D Circle. Can also be used to create a SimpleMesh
 * of any regular polygon.
 */
class CircleMesh : public SimpleMesh {
public:
  /**
   * @brief Construct a new CircleMesh
   *
   * @param sides The number of sides used to approximate the Circle.
   * @param radius The radius of the Circle in model space.
   */
  CircleMesh(int sides, float radius) {
    std::vector<float> verts;
    set_draw_method(DrawMethod::TRIANGLE_FAN);

    verts.push_back(0.0f);
    verts.push_back(0.0f);

    for (int i = 0; i < sides; i++) {
      float a = 2.0f * 3.141592653f * (float(i) / float(sides));
      verts.push_back(radius * cos(a));
      verts.push_back(radius * sin(a));
    }
    verts.push_back(radius);
    verts.push_back(0.0f);

    Scoped<Buffer<float>> vertex_buffer =
        Renderer::gen_buffer<float>(&verts[0], verts.size() * sizeof(float));
    vertex_buffer->set_format({{AttributeType::POSITION_2D, "position"}});

    add_geometry_buffer(std::move(vertex_buffer));
  }
};
} // namespace mare

#endif