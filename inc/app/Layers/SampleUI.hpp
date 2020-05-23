#ifndef SAMPLEUI
#define SAMPLEUI

// MARE
#include "mare/Entities/UI/SliderUI.hpp"
#include "mare/Layer.hpp"
#include "mare/Renderer.hpp"

namespace mare {
/**
 * @brief An example User Interface Layer with a Slider Element
 *
 */
class SampleUI : public Layer {
public:
  SampleUI() : Layer(ProjectionType::ORTHOGRAPHIC) {
    // Create Widgets
    gen_entity<SliderUI>(this);
    get_entity<SliderUI>()->set_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    get_entity<SliderUI>()->set_value(0.5f);
  }

  void on_enter() override {}

  void render(float delta_time) override {
    // Renderer properties
    Renderer::enable_depth_testing(false);
    Renderer::enable_face_culling(true);
  }

  void on_exit() override {}
};
} // namespace mare

#endif