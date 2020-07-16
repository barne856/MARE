#ifndef GRIDMESH
#define GRIDMESH

// MARE
#include "Meshes.hpp"
#include "Renderer.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {
/**
 * @brief A SimpleMesh of a 2D Grid. Includes 2D vertices and no normals or
 * texture coords.
 */
class GridMesh : public SimpleMesh {
public:
  /**
   * @brief Construct a new GridMesh.
   * @details A Grid of (n+1)*(m+1) vertices is constructed and indexed for
   * rendering with triangles.
   *
   * @param n The number of divisions in the x direction, n = horizontal
   * vertices - 1.
   * @param m The number of divisions in the y direction, m = vertical vertices
   * - 1.
   */
  GridMesh(uint32_t n, uint32_t m){
    std::vector<float> verts{};
    std::vector<uint32_t> indes{};
    set_draw_method(DrawMethod::TRIANGLE_STRIP);

    // vertex data
    float left = -0.5f;
    float top = 0.5f;
    float dx = 1.0f / static_cast<float>(n);
    float dy = 1.0f / static_cast<float>(m);
    for (uint32_t j = 0; j < m + 1; j++) {
      for (uint32_t i = 0; i < n + 1; i++) {
        float x = left + static_cast<float>(i) * dx;
        float y = top - static_cast<float>(j) * dy;
        verts.push_back(x);
        verts.push_back(y);
      }
    }
    // index data
    gen_indices(0, n, 0, m, n + 1, 128, indes);

    Referenced<Buffer<float>> vertex_buffer = Renderer::gen_buffer<float>(
        &verts[0], verts.size() * sizeof(float), BufferType::READ_WRITE);
    vertex_buffer->set_format({{AttributeType::POSITION_2D, "position"}});

    Referenced<Buffer<uint32_t>> index_buffer = Renderer::gen_buffer<uint32_t>(
        &indes[0], indes.size() * sizeof(uint32_t), BufferType::READ_WRITE);

    add_geometry_buffer(vertex_buffer);
    set_index_buffer(index_buffer);
    indes.clear();
  }
  /**
   * @brief Generate mesh indices for optimal vertex caching
   *
   * @param x0 The starting column index of the vertices in the grid
   * @param x1 The ending column index of the vertices in the grid
   * @param y0 The starting row index of the vertices in the grid
   * @param y1 The ending row index of the vertices in the grid
   * @param width The number of columns of the vertices in the grid
   * @param cache_size The size of the post translation vertex cache of the
   * hardware
   */
  void gen_indices(int x0, int x1, int y0, int y1, int width, int cache_size, std::vector<uint32_t>& indes) {
    if (x1 - x0 + 1 < cache_size) {
      if (2 * (x1 - x0) + 1 > cache_size) {
        indes.push_back(x0);
        for (int x = x0; x < x1 + 1; x++) {
          indes.push_back(x);
        }
        indes.push_back(UINT32_MAX);
      }

      for (int y = y0; y < y1; y++) {
        for (int x = x0; x < x1 + 1; x++) {
          indes.push_back(width * y + x);
          indes.push_back(width * (y + 1) + x);
        }
        indes.push_back(UINT32_MAX);
      }
    } else {
      int xm = x0 + cache_size - 2;
      gen_indices(x0, xm, y0, y1, width, cache_size, indes);
      gen_indices(xm, x1, y0, y1, width, cache_size, indes);
    }
  }
};
} // namespace mare

#endif