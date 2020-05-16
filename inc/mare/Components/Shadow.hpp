#ifndef SHADOW
#define SHADOW

// MARE
#include "mare/Buffers.hpp"
#include "mare/Components/RenderPack.hpp"
#include "mare/Entities/Camera.hpp"
#include "mare/Mare.hpp"

// External Libraries
#include "glm.hpp"

namespace mare {

// Component to cast and recieve shadows to/from a shadowmap
class Shadow : virtual public RenderPack {
public:
  Shadow() {
    scale_bias_matrix = glm::mat4(
        glm::vec4(0.5f, 0.0f, 0.0f, 0.0f), glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
        glm::vec4(0.0f, 0.0f, 0.5f, 0.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
  }
  Referenced<Camera> light_view;
  glm::mat4 scale_bias_matrix;
  Referenced<Framebuffer> depth_buffer;
};
} // namespace mare

#endif