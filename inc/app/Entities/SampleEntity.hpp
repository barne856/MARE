#ifndef SAMPLEENTITY
#define SAMPLEENTITY

// MARE
#include "mare/Assets/Meshes/TorusMesh.hpp"
#include "mare/Assets/Materials/PhongMaterial.hpp"
#include "mare/Systems.hpp"
#include "mare/Renderer.hpp"
#include "mare/Entity.hpp"
#include "mare/Components/Shadow.hpp"
#include "mare/Systems/Rendering/ShadowRenderer.hpp"

namespace mare
{
    class SampleEntity : public Entity, public Shadow
    {
    public:
        SampleEntity(unsigned int n_segments, unsigned int n_rings, float inner_radius, float outer_radius)
        {
            auto mesh = gen_ref<TorusMesh>(n_segments, n_rings, inner_radius, outer_radius);
            auto material = gen_ref<PhongMaterial>();
            push_packet({mesh, material});
            gen_system<ShadowRenderer>();
        }
    };

} // namespace mare

#endif