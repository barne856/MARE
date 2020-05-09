#ifndef SAMPLEPARTICLESYSTEM
#define SAMPLEPARTICLESYSTEM

// MARE
#include "mare/Buffers.hpp"
#include "mare/Entity.hpp"
#include "mare/Mare.hpp"
#include "mare/Meshes.hpp"
#include "mare/Renderer.hpp"
#include "mare/Shader.hpp"
#include "mare/Systems.hpp"

#include "mare/Materials/BasicMaterial.hpp"
#include "mare/Meshes/SphereMesh.hpp"

#include "gtc/random.hpp"

static const int P_COUNT = 1280;

namespace mare {

// forward declare components
class SampleParticleSystemRenderer;
class SampleParticleSystemPhysics;

class SampleParticleSystem : public Entity {
public:
  SampleParticleSystem() {
    gen_component<SampleParticleSystemRenderer>();
    gen_component<SampleParticleSystemPhysics>();
    velocities = Renderer::gen_buffer<glm::vec3>(
        nullptr, sizeof(glm::vec3) * P_COUNT, BufferType::READ_WRITE);
    velocities->clear(glm::vec3(0.0f));
    models_out = Renderer::gen_buffer<glm::mat4>(
        nullptr, sizeof(glm::mat4) * P_COUNT, BufferType::READ_WRITE);

    gen_asset<InstancedMesh>(P_COUNT); // particles
    gen_asset<BasicMaterial>();        // particle material
    gen_asset<ComputeProgram>("./res/Shaders/NBodyGravity");

    get_asset<BasicMaterial>()->set_color(glm::vec4(0.33f, 0.12f, 0.7f, 1.0f));
    auto particles = get_asset<InstancedMesh>();
    particles->set_mesh(gen_scoped<SphereMesh>(2, 0.02f));
    for (int i = 0; i < P_COUNT; i++) {
      glm::vec3 pos = glm::linearRand(glm::vec3(-1.0f), glm::vec3(1.0f));
      particles->push_instance(glm::translate(glm::mat4(1.0f), pos));
      (*models_out)[i] = glm::translate(glm::mat4(1.0f), pos);
    }

    G = 0.0002f;
  }
  Referenced<Buffer<glm::mat4>> models_out;
  Referenced<Buffer<glm::vec3>> velocities;
  float G;
};

class SampleParticleSystemRenderer : public RenderSystem<SampleParticleSystem> {
public:
  void render(SampleParticleSystem *sample_particle_system, Camera *camera,
              float dt) override {
    Renderer::wireframe_mode(true);
    auto mesh = sample_particle_system->get_asset<InstancedMesh>();
    auto material = sample_particle_system->get_asset<BasicMaterial>();
    mesh->render(camera, material, sample_particle_system->get_model());
    Renderer::wireframe_mode(false);
  }
};

class SampleParticleSystemPhysics : public PhysicsSystem<SampleParticleSystem> {
public:
  void update(SampleParticleSystem *sample_particle_system, float dt) override {
    auto program = sample_particle_system->get_asset<ComputeProgram>();
    auto velocities = sample_particle_system->velocities.get();
    auto models_out = sample_particle_system->models_out.get();
    auto particles = sample_particle_system->get_asset<InstancedMesh>();

    program->bind();
    program->upload_float("dt", dt);
    program->upload_float("G", sample_particle_system->G);

    program->upload_storage("particle_velocities", velocities);
    program->upload_storage("model_instances_out", models_out);
    program->upload_storage("model_instances",
                            particles->get_instance_models());

    program->dispatch_compute(P_COUNT);

    // swap buffers
    sample_particle_system->models_out = particles->swap_instance_models(sample_particle_system->models_out);
  }
};

} // namespace mare

#endif