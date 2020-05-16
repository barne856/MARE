#ifndef SAMPLEPARTICLESCENE
#define SAMPLEPARTICLESCENE

// MARE
#include "app/Entities/SampleParticleSystem.hpp"
#include "mare/Renderer.hpp"
#include "mare/Scene.hpp"
#include "mare/Shader.hpp"
#include "app/Layers/SampleUI.hpp"

namespace mare {

class SampleParticleSceneControls;

class SampleParticleScene : public Scene {
public:
  SampleParticleScene() : Scene(ProjectionType::PERSPECTIVE) {
    gen_system<SampleParticleSceneControls>();
    gen_entity<SampleParticleSystem>();
    t = 0.0f;
    r = 2.0f;
    float x = r * cos(t);
    float y = r * sin(t);
    float z = r * sin(t);
    set_position(glm::vec3(x, y, z));
    look_at(glm::vec3(0.0f));

    gen_layer<SampleUI>();
    get_layer<SampleUI>()->get_entity<SliderUI>()->set_value(0.0f);
    get_layer<SampleUI>()->get_entity<SliderUI>()->set_color(glm::vec4(1.0f));
  }

  void on_enter() override {
    // Start with the cursor enabled
    Renderer::set_cursor(CURSOR::ENABLED);
  }

  void render(float delta_time) override {
    // Renderer properties
    Renderer::enable_blending(true);
    Renderer::enable_depth_testing(true);
    Renderer::enable_face_culling(false);
    // Clear color and depth buffer
    Renderer::clear_color_buffer(bg_color);
    Renderer::clear_depth_buffer();

    // rotate view
    t += delta_time / 10.0f;
    float x = r * cos(t);
    float y = r * sin(t);
    float z = r * sin(t);
    set_position(glm::vec3(x, y, z));
    look_at(glm::vec3(0.0f));

    // Set G
    get_entity<SampleParticleSystem>()->G = 0.001f*std::get<float>(get_layer<SampleUI>()->get_entity<SliderUI>()->get_value());
  }

  void on_exit() override {
    // Show cursor on new scenes
    Renderer::set_cursor(CURSOR::ENABLED);
  }

private:
  glm::vec4 bg_color{0.0f, 0.0f, 0.0f, 0.5f};
  float t;
  float r;
};

class SampleParticleSceneControls : public ControlsSystem<SampleParticleScene> {
public:
  bool on_key(const RendererInput &input, SampleParticleScene *scene) override {
    if (input.ESCAPE_JUST_PRESSED) {
      Renderer::end_renderer();
    }
    // event is handled
    return true;
  }
};

} // namespace mare

#endif