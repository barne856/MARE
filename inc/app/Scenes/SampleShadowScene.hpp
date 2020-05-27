#ifndef SAMPLESHADOWSCENE
#define SAMPLESHADOWSCENE

// MARE
#include "app/Entities/SampleEntity.hpp"
#include "app/Layers/SampleUI.hpp"
#include "app/Layers/SampleWidgetsLayer.hpp"
#include "mare/Assets/Materials/PhongMaterial.hpp"
#include "mare/Assets/Meshes/TorusMesh.hpp"
#include "mare/Components/Rigidbody.hpp"
#include "mare/Entities/Spotlight.hpp"
#include "mare/Renderer.hpp"
#include "mare/Scene.hpp"
#include "mare/Systems/Controls/FlyControls.hpp"
#include "mare/Systems/Physics/EulerMethod.hpp"
#include "mare/Systems/Rendering/ShadowMap.hpp"

namespace mare {

// forward declare controls
class SampleShadowSceneControls;

/**
 * @brief An example of a Scene with ShadowMapped Entities.
 *
 */
class SampleShadowScene : public Scene, public Rigidbody {
public:
  SampleShadowScene() : Scene(ProjectionType::PERSPECTIVE) {
    // generate and puch systems onto the stack
    fly_controls = gen_ref<FlyControls>();
    push_system(fly_controls);
    gen_system<SampleShadowSceneControls>();
    gen_system<ShadowMap>(2);
    gen_system<EulerMethod>();

    // generate and push layers onto the stack
    gen_layer<SampleUI>();

    // Orient Camera
    set_position(glm::vec3(0.0f, 0.0f, 1.0f));
    face_towards(glm::vec3(0.0f, 0.2f, -1.0f));

    // generate and push entities onto the stack
    gen_entity<SampleEntity>(100, 200, 0.1f, 0.2f);
    light = gen_entity<Spotlight>();
    light_pos = glm::vec3(5.0f, 0.0f, 5.0f);
    light_center = glm::vec3(0.0f);
    light->set_position(light_pos);
    light->look_at(light_center);

    // set material and shadowmap properties
    get_entity<SampleEntity>()
        ->get_packet<TorusMesh, PhongMaterial>()
        .second->set_texture(Renderer::gen_texture2D(
            "./inc/app/Assets/Textures/checkerboard.png"));
    get_entity<SampleEntity>()
        ->get_packet<TorusMesh, PhongMaterial>()
        .second->set_light(light);
    get_system<ShadowMap>()->set_light(light);
    time = 0.0f;
  }

  void on_enter() override {
    // Start with the cursor disabled for Fly Contorls
    Renderer::set_cursor(CursorType::DISABLED);
  }

  void render(float delta_time) override {
    // Renderer properties
    Renderer::enable_blending(true);
    Renderer::enable_depth_testing(true);
    Renderer::enable_face_culling(true);
    // Clear color and depth buffer
    Renderer::clear_color_buffer(bg_color);
    Renderer::clear_depth_buffer();

    // update light position
    float angle = get_layer<SampleUI>()->get_entity<SliderUI>()->get_value();
    // time += delta_time;
    light_pos = glm::vec3(5.0f * cos(5.0f * angle), 5.0f * sin(time), 1.0f);
    light->set_position(light_pos);
    light->look_at(light_center);
  }

  void on_exit() override {
    // Show cursor on new scenes
    Renderer::set_cursor(CursorType::ENABLED);
  }
  Referenced<FlyControls> fly_controls;
  Referenced<Spotlight> light;
  glm::vec3 light_pos;
  glm::vec3 light_center;
  float time;

private:
  glm::vec4 bg_color{0.9f, 0.9f, 0.9f, 1.0f};
};

/**
 * @brief An example of a Scene ControlsSystem
 * @details The Camera is swapped with the light's camera when "C" is pressed.
 * Wireframe rendering is toggled when "T" is pressed. When left control is held
 * down, the cursor becomes visible and UI elements can be used.
 *
 */
class SampleShadowSceneControls : public ControlsSystem<SampleShadowScene> {
public:
  bool on_key(const RendererInput &input, SampleShadowScene *scene) override {
    // swap camera
    if (input.C_JUST_PRESSED) {
      scene->swap_camera(scene->light);
    }

    if (input.T_JUST_PRESSED) {
      Renderer::get_info().wireframe = !(Renderer::get_info().wireframe);
    }
    Renderer::wireframe_mode(Renderer::get_info().wireframe);
    // show mouse and disable controls
    if (input.LEFT_CONTROL_JUST_PRESSED) {
      Renderer::set_cursor(CursorType::ENABLED);
      scene->fly_controls = scene->pull_system<FlyControls>();
      scene->linear_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
    } else if (input.LEFT_CONTROL_RELEASED) {
      if (!scene->get_system<FlyControls>()) {
        Renderer::set_cursor(CursorType::DISABLED);
        scene->push_system(scene->fly_controls);
      }
    }
    // pass though to next callback
    return false;
  }
};

} // namespace mare

#endif