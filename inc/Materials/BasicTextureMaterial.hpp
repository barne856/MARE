#ifndef BASICTEXTUREMATERIAL
#define BASICTEXTUREMATERIAL

// External Libraries
#include "glm.hpp"

// MARE
#include "Mare.hpp"
#include "Shader.hpp"

namespace mare {
/**
 * @brief A Basic Material that will render a Texture2D on a Mesh with texture
 * coordinates.
 * @details Does not provide any lighting.
 */
class BasicTextureMaterial : public virtual Material {
public:
  /**
   * @brief Construct a new Basic Texture Material
   *
   */
  BasicTextureMaterial() : Material("./MARE/res/Shaders/BasicTexture") {}
  /**
   * @brief Destroy the Basic Texture Material
   *
   */
  virtual ~BasicTextureMaterial() {}
  /**
   * @brief Upload the Texture2D to the shader when rendered.
   *
   */
  void render() override { upload_texture2D("tex", texture_.get()); }
  /**
   * @brief Set the Texture2D of the Material.
   *
   * @param texture The Texture2D
   */
  void set_texture(Referenced<Texture2D> texture) { texture_ = texture; }

private:
  Referenced<Texture2D>
      texture_; /**< The Referenced Texture2D of the Material.*/
};
} // namespace mare

#endif