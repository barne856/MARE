#ifndef GRIDMESH
#define GRIDMESH

// MARE
#include "Meshes.hpp"
#include "Renderer.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {
/**
 * @brief A SimpleMesh of a 2D Grid, can be used for cloth or terrain. Includes
 * texture coordinates and normals
 */
class GridMesh : public SimpleMesh {
public:
  /**
   * @brief Construct a new GridMesh.
   * @details A Grid of (n+1)*(m+1) vertices is constructed and indexed for
   * rendering with triangles.
   *
   * @param n The number of divisions in the x direction.
   * @param m The number of divisions in the y direction.
   */
  GridMesh(uint32_t n, uint32_t m) {
    std::vector<float> verts;
    std::vector<uint32_t> indes;
    std::vector<float> norms;
    std::vector<float> tex;
    set_draw_method(DrawMethod::TRIANGLES);
    
    // vertex data
    float left = -0.5f;
    float top = 0.5f;
    float dx = 1.0f/static_cast<float>(n);
    float dy = 1.0f/static_cast<float>(m);
    for(uint32_t j = 0; j < m+1; j++)
    {
        for(uint32_t i = 0; i < n+1; i++)
        {
            verts.push_back(left+static_cast<float>(i)*dx);
            verts.push_back(top-static_cast<float>(j)*dy);
            verts.push_back(0.0f);
            norms.push_back(0.0f);
            norms.push_back(0.0f);
            norms.push_back(1.0f);
            tex.push_back(static_cast<float>(i)*dx);
            tex.push_back(static_cast<float>(j)*dy);
        }
    }
    //index data
    // for each square in the grid
    for(uint32_t row = 0; row < m; row++)
    {
        for(uint32_t col = 0; col < n; col++)
        {
            // indices of the square's corners
            uint32_t bottom_left = (row+1)*(n+1) + col;
            uint32_t bottom_right = (row+1)*(n+1) + col+1;
            uint32_t top_right = row*(n+1) + col+1;
            uint32_t top_left = row*(n+1) + col;
            indes.push_back(bottom_left);
            indes.push_back(bottom_right);
            indes.push_back(top_right);
            indes.push_back(bottom_left);
            indes.push_back(top_right);
            indes.push_back(top_left);
        }
    }

    Referenced<Buffer<float>> vertex_buffer =
        Renderer::gen_buffer<float>(&verts[0], verts.size() * sizeof(float));
    vertex_buffer->set_format({{AttributeType::POSITION_3D, "position"}});

    Referenced<Buffer<float>> normal_buffer =
        Renderer::gen_buffer<float>(&norms[0], norms.size() * sizeof(float));
    normal_buffer->set_format({{AttributeType::NORMAL, "normal"}});

    Referenced<Buffer<float>> tex_coord_buffer =
        Renderer::gen_buffer<float>(&tex[0], tex.size() * sizeof(float));
    tex_coord_buffer->set_format({{AttributeType::TEXTURE_MAP, "texcoords"}});

    Referenced<Buffer<uint32_t>> index_buffer =
        Renderer::gen_buffer<uint32_t>(&indes[0], indes.size() * sizeof(float));

    add_geometry_buffer(vertex_buffer);
    add_geometry_buffer(normal_buffer);
    add_geometry_buffer(tex_coord_buffer);
    set_index_buffer(index_buffer);
  }
};
} // namespace mare

#endif