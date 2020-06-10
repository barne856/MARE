#ifndef TRIANGLEMESH
#define TRIANGLEMESH

// MARE
#include "Meshes.hpp"
#include "Renderer.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {
/**
 * @brief A SimpleMesh of a 2D Triangle.
 * @details Provides vertices but no normals or texture coordinates.
 */
class TriangleMesh : public SimpleMesh {
public:
  /**
   * @brief Construct a new TriangleMesh.
   * @details If vertices are given in counter-clockwise order, the Mesh will
   * face the positive z direction. If the vertices are given in clockwise
   * order, the Mesh will face the negative z direction.
   *
   * @param v1 The first vertex.
   * @param v2 The second vertex.
   * @param v3 The third vertex.
   */
  TriangleMesh(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3) {
    std::vector<float> verts;
    set_draw_method(DrawMethod::TRIANGLES);

    verts.push_back(v1[0]);
    verts.push_back(v1[1]);
    verts.push_back(v2[0]);
    verts.push_back(v2[1]);
    verts.push_back(v3[0]);
    verts.push_back(v3[1]);

    Scoped<Buffer<float>> vertex_buffer =
        Renderer::gen_buffer<float>(&verts[0], verts.size() * sizeof(float));
    vertex_buffer->set_format({{AttributeType::POSITION_2D, "position"}});

    add_geometry_buffer(std::move(vertex_buffer));
  }
};
} // namespace mare

#endif