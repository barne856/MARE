#ifndef SAMPLEWIDGETSLAYER
#define SAMPLEWIDGETSLAYER

// MARE
#include "mare/Entities/UI/ColorPicker.hpp"
#include "mare/Layer.hpp"
#include "mare/Renderer.hpp"

#include <iostream>

namespace mare {
/**
 * @brief An example User Interface Layer
 *
 */
class SampleWidgetsLayer : public Layer {
public:
  SampleWidgetsLayer() : Layer(ProjectionType::ORTHOGRAPHIC) {
    // Create Widgets
    auto picker = gen_entity<ColorPicker>(this);
    //picker->set_scale(glm::vec3(0.4f));
    //picker->set_position(glm::vec3(-16.0f/9.0f+0.4f*0.6f, 1.0f-0.4f*0.6f, 0.0f));
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