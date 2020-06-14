#ifndef BILLBOARD
#define BILLBOARD

// MARE
#include "Materials/PhongMaterial.hpp"
#include "Meshes/CharMesh.hpp"
#include "Components/RenderPack.hpp"
#include "Entities/Spotlight.hpp"
#include "Entity.hpp"
#include "Systems/Rendering/PacketRenderer.hpp"

namespace mare {

// forward declare systems
class BillboardRenderer;

/**
 * @brief A Text Billboard that will face the camera
 *
 */
class Billboard : public RenderPack {
public:
  /**
   * @brief Construct a new Billboard object
   *
   * @param str The string used to construct the Billboard.
   * @param stroke_thickness The stroke thickness of the text.
   * @param extrusion_depth The depth of the extrusion of the text.
   * @param max_strokes The maximum number of strokes for the Buffer if the
   * Billboard is dynamic. If 0, the Billboard will be static and not able to be
   * updated.
   * @see CharMesh
   */
  Billboard(std::string str, float stroke_thickness = 0.0f,
            float extrusion_depth = 0.0f, unsigned int max_strokes = 0) {
    text_mesh =
        gen_ref<CharMesh>(str, stroke_thickness, extrusion_depth, max_strokes);
    text_mesh->set_center(glm::vec3(0.0f));
    material = gen_ref<PhongMaterial>();
    auto light = gen_ref<Spotlight>();
    auto light_pos = glm::vec3(5.0f, 0.0f, 5.0f);
    auto light_center = glm::vec3(0.0f);
    light->set_position(light_pos);
    light->face_towards(light_center, {0.0f, 0.0f, 1.0f});
    material->set_light(light);
    push_packet({text_mesh, material});
    gen_system<BillboardRenderer>();
    gen_system<PacketRenderer>();
    normal_vector = {0.0f, 0.0f, 1.0f};
  }
  /**
   * @brief Set the ambient color of the PhongMaterial used to render the
   * Billboard.
   *
   * @param color The color to set.
   */
  void set_color(glm::vec4 color) { material->set_ambient_color(color); }
  /**
   * @brief Set the text of the Billboard. The Billboard must be dynamic and
   * initialized with a number larger than zero for the max_strokes parameter.
   *
   * @param str The string to set.
   */
  void set_text(std::string str) {
    text_mesh->set_text(str);
    text_mesh->set_center(glm::vec3(0.0f));
  }
  glm::vec3 normal_vector; // The normal vector of the horizontal plane.

private:
  Referenced<CharMesh> text_mesh;
  Referenced<PhongMaterial> material;
};

class BillboardRenderer : public RenderSystem<Billboard> {
public:
  void render(float dt, Camera *camera, Billboard *billboard) override {
    billboard->set_rotation_matrix(camera->get_rotation_matrix());
  }
};

} // namespace mare

#endif