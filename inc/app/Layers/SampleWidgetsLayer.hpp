#ifndef SAMPLEWIDGETSLAYER
#define SAMPLEWIDGETSLAYER

// MARE
#include "mare/Entities/UI/ColorPicker.hpp"
#include "mare/Entities/UI/TextBox.hpp"
#include "mare/Layer.hpp"
#include "mare/Renderer.hpp"

namespace mare {
/**
 * @brief An example User Interface Layer
 *
 */
class SampleWidgetsLayer : public Layer {
public:
  SampleWidgetsLayer() : Layer(ProjectionType::ORTHOGRAPHIC) {
    // Create Widgets
    picker = gen_entity<ColorPicker>(this, true);
    picker->set_scale(glm::vec3(0.5f));
    picker->set_position(
        glm::vec3(-16.0f / 9.0f + 0.5f * 0.6f, 1.0f - 0.5f * 0.6f, 0.0f));
    util::Rect bounds, margins;
    float margin = 0.05f;
    bounds.left() = -0.25f;
    bounds.right() = 0.25f;
    bounds.bottom() = -0.038f;
    bounds.top() = 0.038f;
    margins.left() = bounds.left() - margin;
    margins.right() = bounds.right() + margin;
    margins.bottom() = bounds.bottom() - margin;
    margins.top() = bounds.top() + margin;
    texbox = gen_entity<TextBox>(this, bounds, margins, 1, 2048);
    texbox->pin_top_left_corner({-16.0f / 9.0f + 0.05f, 1.0f - 0.65f});
  }

  void on_enter() override {}

  void render(float delta_time) override {
    // Renderer properties
    Renderer::enable_depth_testing(false);
    Renderer::enable_face_culling(true);
    texbox->set_text(util::rgb_to_string(picker->get_value()));
  }

  void on_exit() override {}
  Referenced<TextBox> texbox;
  Referenced<ColorPicker> picker;
  std::string str;
  float t = 0.0f;
  int frames = 0;
};
} // namespace mare

#endif