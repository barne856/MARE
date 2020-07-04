#ifndef TORUSMESH
#define TORUSMESH

// MARE
#include "Meshes.hpp"
#include "Renderer.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {
/**
 * @brief A SimpleMesh of a Torus.
 * @details Provides vertices, normals, and texture coordinates.
 *
 */
class TorusMesh : public SimpleMesh {
public:
  /**
   * @brief Construct a new TorusMesh.
   *
   * @param n_segments The number of segments in each ring.
   * @param n_rings The number of rings in the torus.
   * @param inner_radius The inner radius of the torus in model space.
   * @param outer_radius The outer radius of the torus in model space.
   */
  TorusMesh(unsigned int n_segments, unsigned int n_rings, float inner_radius,
            float outer_radius)
      : r(outer_radius), R(inner_radius), q(n_segments), p(n_rings) {
    std::vector<float> vertex_data;
    std::vector<unsigned int> indices;
    set_draw_method(DrawMethod::TRIANGLE_STRIP);

    // Compute torus vertices and normals
    for (unsigned int i = 0; i <= p; ++i) {
      for (unsigned int j = 0; j <= q; ++j) {
        // Compute texture coordinates (surface parameters)
        // Note that u is added to j. This creates a spiral pattern, which
        // allows us to draw the entire torus in one triangle strip without
        // using degenerate triangles.
        // (Yet we still need degenerate triangles to avoid texture
        // wrap-around)
        float u = float(i) / p;
        float v = (float(j) + u) / q;

        // Compute angles
        float u_angle = u * 2 * PI;
        float v_angle = v * 2 * PI;

        // Position
        float x = cos(u_angle) * (r + cos(v_angle) * R);
        float y = sin(u_angle) * (r + cos(v_angle) * R);
        float z = sin(v_angle) * R;

        // Normal vector
        float nx = cos(u_angle) * cos(v_angle);
        float ny = sin(u_angle) * cos(v_angle);
        float nz = sin(v_angle);

        // add vertex data to vector
        vertex_data.push_back(x);  // position
        vertex_data.push_back(y);  // position
        vertex_data.push_back(z);  // position
        vertex_data.push_back(nx); // normal
        vertex_data.push_back(ny); // normal
        vertex_data.push_back(nz); // normal
        vertex_data.push_back(u);  // texture coord
        vertex_data.push_back(v);  // texture coord
      }
    }

    // Compute torus indices
    unsigned int n_vertices = static_cast<unsigned int>(vertex_data.size() / 8);
    for (unsigned int i = 0; i <= n_vertices; ++i) {
      indices.push_back(static_cast<unsigned int>(i % n_vertices));
      indices.push_back(static_cast<unsigned int>((i + q + 1) % n_vertices));
    }

    Scoped<Buffer<float>> vertex_buffer = Renderer::gen_buffer<float>(
        &vertex_data[0], vertex_data.size() * sizeof(float));
    vertex_buffer->set_format({{AttributeType::POSITION_3D, "position"},
                               {AttributeType::NORMAL, "normal"},
                               {AttributeType::TEXTURE_MAP, "texcoords"}});

    Scoped<Buffer<unsigned int>> index_buffer = Renderer::gen_buffer<uint32_t>(
        &indices[0], indices.size() * sizeof(float));

    add_geometry_buffer(std::move(vertex_buffer));
    set_index_buffer(std::move(index_buffer));
  }

private:
  float r;        // outer radius
  float R;        // inner radius
  unsigned int p; // number of mesh columns
  unsigned int q; // number of mesh row
  const float PI = 3.141592653f;

  // Fuctions to map the grid vertex (u_i,v_j) to the mesh vertex (f(u_i,v_j),
  // g(u_i,v_j), h(u_i,v_j)) on the torus.
  float f(int i, int j) {
    return ((R + r * cos((-1 + 2 * (float)j / q) * PI)) *
            cos((-1 + 2 * (float)i / p) * PI));
  }

  float g(int i, int j) {
    return ((R + r * cos((-1 + 2 * (float)j / q) * PI)) *
            sin((-1 + 2 * (float)i / p) * PI));
  }

  float h(int i, int j) { return (r * sin((-1 + 2 * (float)j / q) * PI)); }
};
} // namespace mare

#endif