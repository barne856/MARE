#ifndef ARRAYMESH
#define ARRAYMESH

// MARE
#include "mare/Meshes.hpp"
#include "mare/Renderer.hpp"

// Standard Library
#include <iostream>

// External Libraries
#include "glm.hpp"

namespace mare {

/**
 * @brief A SimpleMesh class used to create a SimpleMesh from user provided
 * vertices, normals, texture coordinates, and indices.
 */
class ArrayMesh : public SimpleMesh {
public:
  /**
   * @brief Construct a new ArrayMesh
   *
   * @param method The DrawMethod used to render the Mesh.
   * @param vertices The vertices of the Mesh.
   * @param normals The normals of the Mesh.
   * @param texture_coords The texture coordinates of the Mesh.
   * @param indices The indices used to render the Meshes vertices.
   * @see DrawMethod
   */
  ArrayMesh(DrawMethod method, const std::vector<glm::vec3> &vertices,
            const std::vector<glm::vec3> &normals,
            const std::vector<glm::vec2> &texture_coords,
            std::vector<uint32_t> *indices) {
    if (normals.size() != vertices.size()) {
      std::cerr << "normals must be the same size as vertices" << std::endl;
      return;
    }

    if (texture_coords.size() != vertices.size()) {
      std::cerr << "texture_coords must be the same size as vertices"
                << std::endl;
      return;
    }

    // interleave data
    std::vector<float> vertex_data{};
    for (size_t i = 0; i < vertices.size(); i++) {
      vertex_data.push_back(vertices[i][0]);
      vertex_data.push_back(vertices[i][1]);
      vertex_data.push_back(normals[i][0]);
      vertex_data.push_back(normals[i][1]);
      vertex_data.push_back(normals[i][2]);
      vertex_data.push_back(texture_coords[i][0]);
      vertex_data.push_back(texture_coords[i][1]);
    }
    Scoped<Buffer<float>> vb = Renderer::gen_buffer<float>(
        &vertex_data[0], vertex_data.size() * sizeof(float));
    vb->set_format({{AttributeType::POSITON_3D, "position"},
                    {AttributeType::NORMAL, "normals"},
                    {AttributeType::TEXTURE_MAP, "texture_coords"}});
    add_geometry_buffer(std::move(vb));
    if (indices) {
      Scoped<Buffer<uint32_t>> ib = Renderer::gen_buffer<uint32_t>(
          &(indices->front()), indices->size() * sizeof(uint32_t));
      set_index_buffer(std::move(ib));
    }
    set_draw_method(method);
  }
};

} // namespace mare

#endif