#ifndef SAMPLEWIDGETSCENE
#define SAMPLEWIDGETSCENE

// MARE
#include "app/Entities/SampleEntity.hpp"
#include "app/Layers/SampleWidgetsLayer.hpp"
#include "mare/Renderer.hpp"
#include "mare/Scene.hpp"

namespace mare {

/**
 * @brief An example of a Scene with the SampleWidgetsLayer
 *
 */
class SampleWidgetsScene : public Scene {
public:
  SampleWidgetsScene() : Scene(ProjectionType::PERSPECTIVE) {

    // generate and push layers onto the stack
    gen_layer<SampleWidgetsLayer>();

    // Orient Camera
    set_position(glm::vec3(0.0f, 0.0f, 0.5f));
    face_towards(glm::vec3(0.0f, 0.0f, -1.0f));
    set_up_vector({0.0f, 1.0f, 0.0f});
  }

  void on_enter() override {}

  void render(float delta_time) override {
    // Renderer properties
    Renderer::enable_blending(true);
    Renderer::enable_depth_testing(true);
    Renderer::enable_face_culling(true);
    // Clear color and depth buffer
    Renderer::clear_color_buffer(bg_color);
    Renderer::clear_depth_buffer();
  }

  void on_exit() override {}

private:
  glm::vec4 bg_color{0.12f, 0.12f, 0.12f, 1.0f};
};

} // namespace mare

#endif