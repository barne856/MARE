#ifndef PHONGMATERIAL
#define PHONGMATERIAL

// External Libraries
#include "glm.hpp"

// MARE
#include "mare/Buffers.hpp"
#include "mare/Renderer.hpp"
#include "mare/Shader.hpp"

#include "mare/Entities/Spotlight.hpp"

namespace mare {

struct phong_properties {
  glm::vec4 ambient;
  glm::vec4 diffuse;
  glm::vec4 specular;
  float shininess;
};

class PhongMaterial : public virtual Material {
public:
  PhongMaterial() : Material("./inc/mare/Assets/Shaders/Phong") {
    phong_properties props = {glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                              glm::vec4(0.1f, 0.1f, 0.5f, 1.0f),
                              glm::vec4(1.0f), 32.0f};
    properties = Renderer::gen_buffer<phong_properties>(
        &props, sizeof(phong_properties), BufferType::READ_WRITE);
  }
  virtual ~PhongMaterial() {}
  void render() override {
    (*light_props)[0] = spotlight->properties;
    upload_texture2D("tex", texture_.get());
    upload_uniform("material_properties", properties.get());
    upload_uniform("light_properties", light_props.get());
    upload_vec3("light_position", spotlight->get_position());
  }
  void set_texture(Referenced<Texture2D> texture) { texture_ = texture; }
  void set_light(Referenced<Spotlight> light) {
    spotlight = light;
    light_props = Renderer::gen_buffer<light_properties>(
        &(spotlight->properties), sizeof(light_properties), BufferType::READ_WRITE);
  }

private:
  Referenced<Texture2D> texture_;
  Scoped<Buffer<phong_properties>> properties;
  Scoped<Buffer<light_properties>> light_props;
  Referenced<Spotlight> spotlight;
};
} // namespace mare

#endif