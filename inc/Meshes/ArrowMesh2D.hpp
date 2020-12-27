#ifndef ARROWMESH2D
#define ARROWMESH2D

// MARE
#include "Meshes.hpp"
#include "Meshes/QuadrangleMesh.hpp"
#include "Meshes/TriangleMesh.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {
/**
 * @brief A 2D Arrow Mesh composed of Triangle and Quadrangle SimpleMeshes.
 */
class ArrowMesh2D : public CompositeMesh {
public:
  /**
   * @brief Construct a new ArrowMesh2D
   * @details The origin of the Arrow is at the bottom of the body
   *
   * @param body_length The length of the arrow tail.
   * @param body_width The width of the arrow tail.
   * @param head_length The length of the arrow head.
   * @param head_width The width of the arrow head.
   */
  ArrowMesh2D(float body_length, float body_width, float head_length,
              float head_width) {
    glm::vec2 v1 = {head_width / 2.0f, body_length};
    glm::vec2 v2 = {0.0f, body_length + head_length};
    glm::vec2 v3 = {-head_width / 2.0f, body_length};
    head = gen_ref<TriangleMesh>(v1, v2, v3);
    v1 = {-body_width / 2.0f, 0.0f};
    v2 = {body_width / 2.0f, 0.0f};
    v3 = {body_width / 2.0f, body_length};
    glm::vec2 v4 = {-body_width / 2.0f, body_length};
    body = gen_ref<QuadrangleMesh>(v1, v2, v3, v4);
    push_mesh(body);
    push_mesh(head);
  }

private:
  Referenced<TriangleMesh> head;   /**< The head of the arrow.*/
  Referenced<QuadrangleMesh> body; /**< The tail of the arrow. */
};
} // namespace mare

#endif