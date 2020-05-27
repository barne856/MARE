#ifndef ARROWMESH
#define ARROWMESH

// MARE
#include "mare/Assets/Meshes/ConeMesh.hpp"
#include "mare/Assets/Meshes/CylinderMesh.hpp"
#include "mare/Meshes.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {
/**
 * @brief A 3D Arrow Mesh composed of Cone and Cylinder SimpleMeshes.
 */
class ArrowMesh : public CompositeMesh {
public:
  /**
   * @brief Construct a new ArrowMesh
   *
   * @param body_length The length of the arrow tail.
   * @param body_width The width of the arrow tail.
   * @param head_length The length of the arrow head.
   * @param head_width The width of the arrow head.
   * @param sides The sides used to approximate the Cylinder and Cone Meshes.
   */
  ArrowMesh(float body_length, float body_width, float head_length,
            float head_width, unsigned int sides) {
    head = gen_ref<ConeMesh>(0.5f, sides);
    body = gen_ref<CylinderMesh>(0.0f, 2.0f * 3.141592653f, sides);
    body->set_scale({body_width, body_width, body_length});
    head->set_scale({head_width, head_width, head_length});
    head->set_position({0.0f, 0.0f, body_length});
    push_mesh(body);
    push_mesh(head);
  }

private:
  Referenced<ConeMesh> head;     /**< The head of the arrow.*/
  Referenced<CylinderMesh> body; /**< The tail of the arrow. */
};
} // namespace mare

#endif