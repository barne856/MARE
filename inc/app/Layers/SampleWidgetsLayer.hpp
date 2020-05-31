#ifndef SAMPLEWIDGETSLAYER
#define SAMPLEWIDGETSLAYER

// MARE
#include "mare/Entities/UI/Button.hpp"
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
    // Color Picker
    picker = gen_entity<ColorPicker>(this, true);
    picker->set_scale(glm::vec3(0.5f));
    picker->set_position(
        glm::vec3(-16.0f / 9.0f + 0.5f * 0.6f, 1.0f - 0.5f * 0.6f, 0.0f));
    
    // Text box
    util::Rect bounds;
    bounds.left() = -0.30f;
    bounds.right() = 0.30f;
    bounds.bottom() = -0.05f;
    bounds.top() = 0.05f;
    // allocate 16kB for the text in the box
    texbox = gen_entity<TextBox>(this, bounds, 2, 0.01f, 0.005f, 16000);
    texbox->pin_top_left_corner({-16.0f / 9.0f + 0.05f, 1.0f - 0.65f});

    // Button
    button = gen_entity<Button>(this, bounds, "Clear Text");
    button->set_on_click_callback(clear_text);
  }

  static void clear_text(Layer* layer)
  {
    auto texbox = layer->get_entity<TextBox>();
    texbox->clear_text();
  }

  void on_enter() override {}

  void render(float delta_time) override {
    // Renderer properties
    Renderer::enable_depth_testing(false);
    Renderer::enable_face_culling(true);
  }

  void on_exit() override {}
  Referenced<TextBox> texbox;
  Referenced<Button> button;
  Referenced<ColorPicker> picker;
  std::string str;
  float t = 0.0f;
  int frames = 0;
};
} // namespace mare

#endif