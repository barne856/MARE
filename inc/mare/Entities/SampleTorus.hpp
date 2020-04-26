#ifndef SAMPLETORUS
#define SAMPLETORUS

// MARE
#include "mare/Meshes/TorusMesh.hpp"
#include "mare/Materials/PhongMaterial.hpp"
#include "mare/Systems.hpp"
#include "mare/Renderer.hpp"
#include "mare/Entity.hpp"

namespace mare
{

// forward declare SampleTorus components
class SampleTorusRenderer;

class SampleTorus : public Entity
{
public:
    SampleTorus(unsigned int n_segments, unsigned int n_rings, float inner_radius, float outer_radius)
    {
        torus = Renderer::API->GenScoped<TorusMesh>(n_segments, n_rings, inner_radius, outer_radius);
        material = Renderer::API->GenScoped<PhongMaterial>();
        push_component<SampleTorusRenderer>();
    }
    Scoped<TorusMesh> torus = nullptr;
    Scoped<PhongMaterial> material = nullptr;
};

class SampleTorusRenderer : public RenderSystem<SampleTorus>
{
public:
    void render(SampleTorus *sample_torus, Layer *layer, float dt) override
    {
        sample_torus->torus->render(layer, sample_torus->material.get(), sample_torus->get_model());
    }
};

} // namespace mare

#endif