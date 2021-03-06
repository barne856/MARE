#ifndef QUADRANGLEMESH
#define QUADRANGLEMESH

// MARE
#include "Components/Widget.hpp"
#include "Meshes.hpp"
#include "Renderer.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {
/**
 * @brief A SimpleMesh of a 2D Quadrangle.
 * @details Provides only vertices and no normals or texture coordinates.
 */
class QuadrangleMesh : public SimpleMesh {
public:
  /**
   * @brief Construct a new QuadrangleMesh centered in model space as a square
   * with length 1 and width 1. Faces towards positive z.
   */
  QuadrangleMesh() {
    std::vector<float> verts;
    set_draw_method(DrawMethod::TRIANGLES);

    verts.push_back(-0.5f);
    verts.push_back(-0.5f);
    verts.push_back(0.5f);
    verts.push_back(-0.5f);
    verts.push_back(0.5f);
    verts.push_back(0.5f);
    verts.push_back(0.5f);
    verts.push_back(0.5f);
    verts.push_back(-0.5f);
    verts.push_back(0.5f);
    verts.push_back(-0.5f);
    verts.push_back(-0.5f);

    Referenced<Buffer<float>> vertex_buffer =
        Renderer::gen_buffer<float>(&verts[0], verts.size() * sizeof(float));
    vertex_buffer->set_format({{AttributeType::POSITION_2D, "position"}});

    add_geometry_buffer(vertex_buffer);
  }
  /**
   * @brief Construct a new QuadrangleMesh.
   * @details Given vertices in counter-clockwise order from bottom left the
   * Mesh will face towards positive z. If vertices are given in clockwise order
   * the Mesh faces negative z.
   *
   * @param v1 The first vertex
   * @param v2 The second vertex
   * @param v3 The third vertex
   * @param v4 The fourth vertex
   */
  QuadrangleMesh(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec2 v4) {
    std::vector<float> verts;
    set_draw_method(DrawMethod::TRIANGLES);

    verts.push_back(v1[0]);
    verts.push_back(v1[1]);
    verts.push_back(v2[0]);
    verts.push_back(v2[1]);
    verts.push_back(v3[0]);
    verts.push_back(v3[1]);
    verts.push_back(v3[0]);
    verts.push_back(v3[1]);
    verts.push_back(v4[0]);
    verts.push_back(v4[1]);
    verts.push_back(v1[0]);
    verts.push_back(v1[1]);

    Referenced<Buffer<float>> vertex_buffer =
        Renderer::gen_buffer<float>(&verts[0], verts.size() * sizeof(float));
    vertex_buffer->set_format({{AttributeType::POSITION_2D, "position"}});

    add_geometry_buffer(vertex_buffer);
  }
  /**
   * @brief Construct a new QuadrangleMesh using a util::Rect.
   *
   * @param bounds The bounds for the quadrangle.
   */
  QuadrangleMesh(util::Rect bounds) {
    std::vector<float> verts;
    set_draw_method(DrawMethod::TRIANGLES);

    verts.push_back(bounds.left());
    verts.push_back(bounds.bottom());
    verts.push_back(bounds.right());
    verts.push_back(bounds.bottom());
    verts.push_back(bounds.right());
    verts.push_back(bounds.top());
    verts.push_back(bounds.right());
    verts.push_back(bounds.top());
    verts.push_back(bounds.left());
    verts.push_back(bounds.top());
    verts.push_back(bounds.left());
    verts.push_back(bounds.bottom());

    Referenced<Buffer<float>> vertex_buffer =
        Renderer::gen_buffer<float>(&verts[0], verts.size() * sizeof(float));
    vertex_buffer->set_format({{AttributeType::POSITION_2D, "position"}});

    add_geometry_buffer(vertex_buffer);
  }
  /**
   * @brief Construct a new QuadrangleMesh with a color on each vertex
   *
   * @param c1 Vertex 1 color (bottom left)
   * @param c2 Vertex 2 color (bottom right)
   * @param c3 Vertex 3 color (top right)
   * @param c4 Vertex 4 color (top left)
   */
  QuadrangleMesh(glm::vec4 c1, glm::vec4 c2, glm::vec4 c3, glm::vec4 c4) {
    std::vector<float> data;
    set_draw_method(DrawMethod::TRIANGLES);

    data.push_back(-0.5f);
    data.push_back(-0.5f);

    data.push_back(c1.r);
    data.push_back(c1.g);
    data.push_back(c1.b);
    data.push_back(c1.a);

    data.push_back(0.5f);
    data.push_back(-0.5f);

    data.push_back(c2.r);
    data.push_back(c2.g);
    data.push_back(c2.b);
    data.push_back(c2.a);

    data.push_back(0.5f);
    data.push_back(0.5f);

    data.push_back(c3.r);
    data.push_back(c3.g);
    data.push_back(c3.b);
    data.push_back(c3.a);

    data.push_back(0.5f);
    data.push_back(0.5f);

    data.push_back(c3.r);
    data.push_back(c3.g);
    data.push_back(c3.b);
    data.push_back(c3.a);

    data.push_back(-0.5f);
    data.push_back(0.5f);

    data.push_back(c4.r);
    data.push_back(c4.g);
    data.push_back(c4.b);
    data.push_back(c4.a);

    data.push_back(-0.5f);
    data.push_back(-0.5f);

    data.push_back(c1.r);
    data.push_back(c1.g);
    data.push_back(c1.b);
    data.push_back(c1.a);

    Referenced<Buffer<float>> vertex_buffer =
        Renderer::gen_buffer<float>(&data[0], data.size() * sizeof(float));
    vertex_buffer->set_format({{AttributeType::POSITION_2D, "position"},
                               {AttributeType::COLOR, "color"}});

    add_geometry_buffer(vertex_buffer);
  }
}; // namespace mare
} // namespace mare

#endif