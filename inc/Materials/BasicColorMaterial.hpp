#ifndef BASICCOLORMATERIAL
#define BASICCOLORMATERIAL

// External Libraries
#include "glm.hpp"

// MARE
#include "Shader.hpp"

namespace mare {
/**
 * @brief A Basic Material that provides a color but no lighting.
 * @details Useful for solid color UI elements or generating a ShadowMap. Only
 * one shader is ever compiled and is shared between all instances of
 * BasicColorMaterial.
 */
class BasicColorMaterial : public virtual Material {
public:
  /**
   * @brief Construct a new Basic Material
   */
  BasicColorMaterial()
      : Material("./MARE/res/Shaders/BasicColor"), m_color(glm::vec4(1.0f)) {}
  virtual ~BasicColorMaterial() {}
  /**
   * @brief uploads the color to the shader when rendered.
   *
   */
  void render() override { upload_vec4("u_color", m_color); }
  /**
   * @brief Set the color of the Material.
   *
   * @param color The color to set.
   */
  inline void set_color(glm::vec4 color) { m_color = color; }

protected:
  glm::vec4 m_color;
};
} // namespace mare

#endif