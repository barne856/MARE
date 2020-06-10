#ifndef SHADOW
#define SHADOW

// MARE
#include "Buffers.hpp"
#include "Components/RenderPack.hpp"
#include "Entities/Camera.hpp"
#include "Mare.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {

/**
 * @brief The Shadow Component allows an Entity to cast and receive shadows from
 * a ShadowMap.
 * @details Every Entity in a Scene that has a ShadowMap System will cast
 * shadows if it has a Shadow Component. If the Entity also has a ShadowRenderer
 * System attached to it, it will also render any shadows cast onto the Entity.
 */
class Shadow : virtual public RenderPack {
public:
  Shadow() {
    scale_bias_matrix = glm::mat4(
        glm::vec4(0.5f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.5f, 0.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  }
  Referenced<Camera> light_view; /**< The view from the light.*/
  glm::mat4 scale_bias_matrix;
  Referenced<Framebuffer>
      depth_buffer; /**< The depth-buffer from the view of the light.*/
};
} // namespace mare

#endif