#ifndef QUADRANGLEMESH
#define QUADRANGLEMESH

// MARE
#include "mare/Meshes.hpp"
#include "mare/Renderer.hpp"

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
    std::vector<float> data;
    set_draw_method(DrawMethod::TRIANGLES);

    data.push_back(-0.5f);
    data.push_back(-0.5f);

    data.push_back(0.0f);
    data.push_back(0.0f);

    data.push_back(0.5f);
    data.push_back(-0.5f);

    data.push_back(1.0f);
    data.push_back(0.0f);

    data.push_back(0.5f);
    data.push_back(0.5f);

    data.push_back(1.0f);
    data.push_back(1.0f);

    data.push_back(0.5f);
    data.push_back(0.5f);

    data.push_back(1.0f);
    data.push_back(1.0f);

    data.push_back(-0.5f);
    data.push_back(0.5f);

    data.push_back(0.0f);
    data.push_back(1.0f);

    data.push_back(-0.5f);
    data.push_back(-0.5f);

    data.push_back(0.0f);
    data.push_back(0.0f);

    Scoped<Buffer<float>> geometry_buffer =
        Renderer::gen_buffer<float>(&data[0], data.size() * sizeof(float));
    geometry_buffer->set_format({{AttributeType::POSITION_2D, "position"},
                                 {AttributeType::TEXTURE_MAP, "texcoords"}});

    add_geometry_buffer(std::move(geometry_buffer));
  }
  /**
   * @brief Construct a new QuadrangleMesh.
   * @details Given vertices in counter-clockwise order the Mesh will face
   * towards positive z. If vertices are given in clockwise order the Mesh faces
   * negative z.
   *
   * @param v1 The first vertex
   * @param v2 The second vertex
   * @param v3 The third vertex
   * @param v4 The fourth vertex
   */
  QuadrangleMesh(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3, glm::vec2 v4) {
    std::vector<float> verts;
    std::vector<float> tex_coords;
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

    tex_coords.push_back(0.0f);
    tex_coords.push_back(0.0f);
    tex_coords.push_back(1.0f);
    tex_coords.push_back(0.0f);
    tex_coords.push_back(1.0f);
    tex_coords.push_back(1.0f);
    tex_coords.push_back(0.0f);
    tex_coords.push_back(1.0f);
    tex_coords.push_back(0.0f);
    tex_coords.push_back(0.0f);

    Scoped<Buffer<float>> vertex_buffer =
        Renderer::gen_buffer<float>(&verts[0], verts.size() * sizeof(float));
    Scoped<Buffer<float>> tex_coord_buffer = Renderer::gen_buffer<float>(
        &tex_coords[0], tex_coords.size() * sizeof(float));
    vertex_buffer->set_format({{AttributeType::POSITION_2D, "position"}});
    tex_coord_buffer->set_format({{AttributeType::TEXTURE_MAP, "texcoords"}});

    add_geometry_buffer(std::move(vertex_buffer));
    add_geometry_buffer(std::move(tex_coord_buffer));
  }
};
} // namespace mare

#endif