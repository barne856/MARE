#ifndef SAMPLEWIDGETSLAYER
#define SAMPLEWIDGETSLAYER

// MARE
#include "mare/Entities/Billboard.hpp"
#include "mare/Entities/UI/Button.hpp"
#include "mare/Entities/UI/ColorPicker.hpp"
#include "mare/Entities/UI/Slider.hpp"
#include "mare/Entities/UI/Switch.hpp"
#include "mare/Entities/UI/TextBox.hpp"
#include "mare/Layer.hpp"
#include "mare/Renderer.hpp"
#include "mare/Systems.hpp"

namespace mare {

// forward declare controls
class SampleWidgetsLayerControls;

/**
 * @brief An example User Interface Layer
 *
 */
class SampleWidgetsLayer : public Layer {
public:
  SampleWidgetsLayer() : Layer(ProjectionType::ORTHOGRAPHIC) {
    util::Rect bounds;
    // Color Picker
    bounds.left() = -0.25f;
    bounds.right() = 0.25f;
    bounds.bottom() = -0.25;
    bounds.top() = 0.25f;
    picker = gen_entity<ColorPicker>(this, bounds, true);

    // Text box
    bounds.bottom() = -0.1f;
    bounds.top() = 0.1f;
    texbox = gen_entity<TextBox>(this, bounds, 2, 0.01f, 0.005f, 4*182);

    // Button
    bounds.bottom() = -0.05f;
    bounds.top() = 0.05f;
    button = gen_entity<Button>(this, bounds, "SET TEXT");
    button->set_on_click_callback(set_text);

    // Slider Bar
    slider = gen_entity<Slider>(this, bounds);
    slider->set_value(0.5f);
    slider->rescale();

    // Switch
    bounds.left() = -0.1f;
    bounds.right() = 0.1f;
    switch_toggle = gen_entity<Switch>(this, bounds);

    rescale();

    gen_system<SampleWidgetsLayerControls>();
  }

  void rescale() {
    float aspect = Renderer::get_info().window_aspect;
    picker->set_position({-aspect + 0.3f, 0.7f, 0.0f});
    texbox->set_position({-aspect + 0.3f, 0.35f, 0.0f});
    button->set_position({-aspect + 0.3f, 0.15f, 0.0f});
    slider->set_position({-aspect + 0.3f, 0.0f, 0.0f});
    switch_toggle->set_position({-aspect + 0.3f, -0.15f, 0.0f});
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
    texbox->set_text("FPS: " + std::to_string(1.0f/delta_time));
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

class SampleWidgetsLayerControls : public ControlsSystem<SampleWidgetsLayer> {
public:
  bool on_resize(const RendererInput &input, SampleWidgetsLayer *layer) {
    layer->rescale();
    return false;
  }
};

} // namespace mare

#endif