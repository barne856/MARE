#ifndef SAMPLEWIDGETSCENE
#define SAMPLEWIDGETSCENE

// MARE
#include "app/Entities/SampleEntity.hpp"
#include "app/Layers/SampleWidgetsLayer.hpp"
#include "mare/Renderer.hpp"
#include "mare/Scene.hpp"

#include "mare/Assets/Materials/BasicMaterial.hpp"
#include "mare/Assets/Materials/PhongMaterial.hpp"
#include "mare/Assets/Meshes/CharMesh.hpp"

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
    set_position(glm::vec3(2.0f, 2.0f, 2.0f));
    // face_towards(glm::vec3(0.0f, -1.0f, 0.0f));
    // set_up_vector({0.0f, 0.0f, 1.0f});
    look_at(glm::vec3(0.0f));

    // Extruded Text
    sample_text =
        gen_ref<CharMesh>("HELLO, WORLD!", 1.0f / 17.0f, 3.0f / 17.0f);
    sample_text->set_scale(glm::vec3(0.3f));
    sample_text->set_center(glm::vec3(0.0f));
    sample_text_material = gen_ref<PhongMaterial>();
    // sample_text_material->set_color(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    auto light = gen_entity<Spotlight>();
    auto light_pos = glm::vec3(5.0f, 0.0f, 5.0f);
    auto light_center = glm::vec3(0.0f);
    light->set_position(light_pos);
    light->look_at(light_center);
    sample_text_material->set_light(light);
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

    //Renderer::wireframe_mode(true);

    t += delta_time / 10.0f;
    float x = r * cos(t);
    float y = r * sin(t);
    float z = r * sin(t);
    set_position(glm::vec3(x, y, z));
    look_at(glm::vec3(0.0f));

    // render sample text
    sample_text->render(this, sample_text_material.get());
  }

  void on_exit() override {}

private:
  glm::vec4 bg_color{0.12f, 0.12f, 0.12f, 1.0f};
  // testing
  Referenced<CharMesh> sample_text;
  Referenced<PhongMaterial> sample_text_material;
  float t = 0.0f;
  float r = 2.0f;
};

} // namespace mare

#endif