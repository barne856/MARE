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
 * vertex data and optional index data.
 */
class ArrayMesh : public SimpleMesh {
public:
  /**
   * @brief Construct a new ArrayMesh
   *
   * @param method The DrawMethod used to render the Mesh.
   * @param vertex_data The Buffers containing the data for the vertices of the Mesh.
   * @param index_data The Buffer containing the index data of the Mesh.
   * @see DrawMethod
   * @see Buffer
   */
  ArrayMesh(DrawMethod method, std::vector<Referenced<Buffer<float>>> vertex_data, Referenced<Buffer<uint32_t>> index_data = nullptr) {
    set_draw_method(method);
    for(auto data : vertex_data)
    {
      add_geometry_buffer(data);
    }
    if(index_data)
    {
      set_index_buffer(index_data);
    }
  }
};

} // namespace mare

#endif