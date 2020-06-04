#ifndef BILLBOARD
#define BILLBOARD

// MARE
#include "mare/Assets/Materials/PhongMaterial.hpp"
#include "mare/Assets/Meshes/CharMesh.hpp"
#include "mare/Components/RenderPack.hpp"
#include "mare/Entity.hpp"
#include "mare/Systems/Rendering/PacketRenderer.hpp"
#include "mare/Entities/Spotlight.hpp"

namespace mare {
class Billboard : public Entity, public RenderPack {
public:
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
    light->look_at(light_center);
    material->set_light(light);
    push_packet({text_mesh, material});
    gen_system<PacketRenderer>();
  }
  void set_color(glm::vec4 color) { material->set_ambient_color(color); }
  void set_text(std::string str) {
    text_mesh->set_text(str);
    text_mesh->set_center(glm::vec3(0.0f));
  }

private:
  Referenced<CharMesh> text_mesh;
  Referenced<PhongMaterial> material;
};
} // namespace mare

#endif