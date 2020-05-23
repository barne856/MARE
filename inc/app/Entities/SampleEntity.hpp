#ifndef SAMPLEENTITY
#define SAMPLEENTITY

// MARE
#include "mare/Assets/Materials/PhongMaterial.hpp"
#include "mare/Assets/Meshes/SphereMesh.hpp"
#include "mare/Assets/Meshes/TorusMesh.hpp"
#include "mare/Components/Shadow.hpp"
#include "mare/Entity.hpp"
#include "mare/Renderer.hpp"
#include "mare/Systems.hpp"
#include "mare/Systems/Rendering/ShadowRenderer.hpp"

namespace mare {
/**
 * @brief An Example of an Entity with a Shadow Component
 * @details This Entity contains a Torus and Sphere rendered with the
 * PhongMaterial.
 * @see Shadow
 * @see Entity
 * @see TorusMesh
 * @see SphereMesh
 * @see PhongMaterial
 * @see ShadowRenderer
 *
 */
class SampleEntity : public Entity, public Shadow {
public:
  SampleEntity(unsigned int n_segments, unsigned int n_rings,
               float inner_radius, float outer_radius) {
    auto mesh1 =
        gen_ref<TorusMesh>(n_segments, n_rings, inner_radius, outer_radius);
    auto material = gen_ref<PhongMaterial>();
    auto mesh2 = gen_ref<SphereMesh>(3, 0.05f);
    mesh2->set_position(glm::vec3(0.0f, 0.0f, 0.4f));
    push_packet({mesh1, material});
    push_packet({mesh2, material});
    gen_system<ShadowRenderer>();
  }
};

} // namespace mare

#endif