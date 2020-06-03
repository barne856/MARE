#ifndef SAMPLEUI
#define SAMPLEUI

// MARE
#include "mare/Entities/UI/Slider.hpp"
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
    util::Rect bounds;
    bounds.left() = -0.5f;
    bounds.right() = 0.5f;
    bounds.top() = 0.5f;
    bounds.bottom() = -0.5f;
    gen_entity<Slider>(this, bounds);
    get_entity<Slider>()->set_value(0.5f);
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