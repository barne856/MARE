#ifndef PHONGMATERIAL
#define PHONGMATERIAL

// External Libraries
#include "glm.hpp"

// MARE
#include "mare/Buffers.hpp"
#include "mare/Entities/Spotlight.hpp"
#include "mare/Renderer.hpp"
#include "mare/Shader.hpp"

namespace mare {

/**
 * @brief Basic Phong-style Material properties.
 *
 */
struct phong_properties {
  glm::vec4 ambient;  /**< Static color not dependent on any lights.*/
  glm::vec4 diffuse;  /**< Directional color depending on the position of the
                         light relative to the Mesh.*/
  glm::vec4 specular; /**< Directional color depending on the reflection of the
                         light off the Mesh towards the Camera's view.*/
  float shininess;    /**< The intensity fo the specular effect.*/
};

/**
 * @brief A Basic Material that provides Phong-Style Lighting and Texturing for
 * Meshes with texture coordinates and normal vectors.
 */
class PhongMaterial : public virtual Material {
public:
  /**
   * @brief Construct a new Phong Material
   */
  PhongMaterial() : Material("./inc/mare/Assets/Shaders/Phong") {
    phong_properties props = {glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                              glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
                              glm::vec4(1.0f), 32.0f};
    properties = Renderer::gen_buffer<phong_properties>(
        &props, sizeof(phong_properties), BufferType::READ_WRITE);
  }
  /**
   * @brief Destroy the Phong Material
   */
  virtual ~PhongMaterial() {}
  /**
   * @brief Upload the material and light properties along with the Texture2D
   * and light position.
   *
   */
  void render() override {
    if (spotlight) {
      (*light_props)[0] = spotlight->properties;
      upload_uniform("light_properties", light_props.get());
      upload_vec3("light_position", spotlight->get_position());
    }
    else
    {
      std::cerr << "ERROR: No light assigned to PhongMaterial!" << std::endl;
    }
    if (texture_) {
      upload_texture2D("tex", texture_.get());
    }
    upload_uniform("material_properties", properties.get());
  }
  /**
   * @brief Set the ambient color of the material.
   *
   * @param color The color to set.
   */
  void set_ambient_color(glm::vec4 color) {
    phong_properties props = {color, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f),
                              glm::vec4(1.0f), 32.0f};
    (*properties.get())[0] = props;
  }
  /**
   * @brief Set the Texture2D of the Material.
   *
   * @param texture The Texture2D to set.
   */
  void set_texture(Referenced<Texture2D> texture) { texture_ = texture; }
  /**
   * @brief Set the light used to render the Phong-style lighting.
   *
   * @param light The Spotlight to set.
   */
  void set_light(Referenced<Spotlight> light) {
    spotlight = light;
    light_props = Renderer::gen_buffer<light_properties>(
        &(spotlight->properties), sizeof(light_properties),
        BufferType::READ_WRITE);
  }

private:
  Referenced<Texture2D> texture_; /**< The Texture2D used by the Material.*/
  Scoped<Buffer<phong_properties>> properties;  /**< The material properties.*/
  Scoped<Buffer<light_properties>> light_props; /**< The light properties.*/
  Referenced<Spotlight>
      spotlight; /**< The Spotlight used to render the Phong-style lighting.*/
};
} // namespace mare

#endif