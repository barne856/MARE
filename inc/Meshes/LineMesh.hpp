#ifndef LINEMESH
#define LINEMESH

#include "Meshes.hpp"

namespace mare {
/**
 * @brief A 2D Line mesh that provides 2D vertices for the line.
 * @details Lines can be drawn with zero thickness or with a positive thickness.
 * Positive thickness will draw lines from quads while a zero
 * thickness will only draw lines 1 pixel thick.
 *
 */
class LineMesh : public SimpleMesh {
public:
  /**
   * @brief Construct a new LineMesh.
   * @details The line will connect (-0.5,0.0) -- (0.5,0.0) in model space and
   * the thickness will be centered along the y-axis.
   *
   * @param thickness The thickness of the line. If zero, the line will take up 1 pixel in width.
   */
  LineMesh(float thickness = 0.0f) {
    assert(thickness >= 0.0f);
    if (thickness == 0.0f) {
      float verts[4] = {-0.5f, 0.0f, 0.5f, 0.0f};
      set_draw_method(DrawMethod::LINES);
      Referenced<Buffer<float>> vertex_buffer =
          Renderer::gen_buffer<float>(verts, sizeof(verts));
      vertex_buffer->set_format({{AttributeType::POSITION_2D, "position"}});
      add_geometry_buffer(vertex_buffer);
    } else {
      float verts[12] = {
          -0.5f, -thickness*0.5f, 0.5f,  -thickness*0.5f, 0.5f,  thickness*0.5f,
          0.5f,  thickness*0.5f,  -0.5f, thickness*0.5f,  -0.5f, -thickness*0.5f,
      };
      set_draw_method(DrawMethod::TRIANGLES);
      Referenced<Buffer<float>> vertex_buffer =
          Renderer::gen_buffer<float>(verts, sizeof(verts));
      vertex_buffer->set_format({{AttributeType::POSITION_2D, "position"}});
      add_geometry_buffer(vertex_buffer);
    }
  }
};
} // namespace mare

#endif