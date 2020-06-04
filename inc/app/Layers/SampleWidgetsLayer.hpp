#ifndef SAMPLEWIDGETSLAYER
#define SAMPLEWIDGETSLAYER

// MARE
#include "mare/Entities/UI/Button.hpp"
#include "mare/Entities/UI/ColorPicker.hpp"
#include "mare/Entities/UI/Slider.hpp"
#include "mare/Entities/UI/Switch.hpp"
#include "mare/Entities/UI/TextBox.hpp"
#include "mare/Layer.hpp"
#include "mare/Renderer.hpp"
#include "mare/Entities/Billboard.hpp"

namespace mare {
/**
 * @brief An example User Interface Layer
 *
 */
class SampleWidgetsLayer : public Layer {
public:
  SampleWidgetsLayer() : Layer(ProjectionType::ORTHOGRAPHIC) {
    util::Rect bounds;
    // Color Picker
    bounds.left() = -0.6f;
    bounds.right() = 0.8f;
    bounds.bottom() = -0.6;
    bounds.top() = 0.6f;
    picker = gen_entity<ColorPicker>(this, bounds, true);
    picker->set_scale(glm::vec3(0.5f));
    picker->set_position(
        glm::vec3(-16.0f / 9.0f + 0.5f * 0.6f, 1.0f - 0.5f * 0.6f, 0.0f));

    // Text box
    bounds.left() = -0.30f;
    bounds.right() = 0.30f;
    bounds.bottom() = -0.05f;
    bounds.top() = 0.05f;
    texbox = gen_entity<TextBox>(this, bounds, 1, 0.01f, 0.005f, 182);
    texbox->pin_top_left_corner({-16.0f / 9.0f + 0.05f, 1.0f - 0.65f});

    // Button
    button = gen_entity<Button>(this, bounds, "SET TEXT");
    button->set_position(glm::vec3(-16.0f / 9.0f + 0.585f * 0.6f, 0.15f, 0.0f));
    button->set_on_click_callback(set_text);

    // Slider Bar
    slider = gen_entity<Slider>(this, bounds);
    slider->set_position(
        glm::vec3(-16.0f / 9.0f + 0.585f * 0.6f, -0.15f, 0.0f));
    slider->set_value(0.5f);
    slider->rescale();

    // Switch
    bounds.left() = -0.1f;
    bounds.right() = 0.1f;
    switch_toggle = gen_entity<Switch>(this, bounds);
    switch_toggle->set_position(
        glm::vec3(-16.0f / 9.0f + 0.585f * 0.6f, 0.0f, 0.0f));
  }

  static void set_text(Layer *layer) {
    auto texbox = layer->get_entity<TextBox>();
    auto scene = Renderer::get_info().scene;
    auto billboard = scene->get_entity<Billboard>();
    std::string str = texbox->get_value();
    billboard->set_text(str);
  }

  void on_enter() override {}

  void render(float delta_time) override {
    // Renderer properties
    Renderer::enable_depth_testing(false);
    Renderer::enable_face_culling(true);
    Renderer::wireframe_mode(false);
  }

  void on_exit() override {}
  Referenced<Slider> slider;
  Referenced<Switch> switch_toggle;
  Referenced<TextBox> texbox;
  Referenced<Button> button;
  Referenced<ColorPicker> picker;
  std::string str;
  float t = 0.0f;
  int frames = 0;
};
} // namespace mare

#endif