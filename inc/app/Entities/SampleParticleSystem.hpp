#ifndef SAMPLEPARTICLESYSTEM
#define SAMPLEPARTICLESYSTEM

// MARE
#include "mare/Assets/Materials/BasicColorMaterial.hpp"
#include "mare/Assets/Meshes/SphereMesh.hpp"
#include "mare/Buffers.hpp"
#include "mare/Components/Transform.hpp"
#include "mare/Entity.hpp"
#include "mare/Mare.hpp"
#include "mare/Meshes.hpp"
#include "mare/Renderer.hpp"
#include "mare/Shader.hpp"
#include "mare/Systems.hpp"

// External Libraries
#include "gtc/random.hpp"

namespace mare {

// forward declare components
class SampleParticleSystemRenderer;
class SampleParticleSystemPhysics;

/**
 * @brief An Example of a particle system.
 */
class SampleParticleSystem : public Entity, public Transform {
public:
  /**
   * @brief Construct a new SampleParticleSystem Entity.
   */
  SampleParticleSystem() {
    gen_system<SampleParticleSystemRenderer>();
    gen_system<SampleParticleSystemPhysics>();
    velocities = Renderer::gen_buffer<glm::vec3>(
        nullptr, sizeof(glm::vec3) * P_COUNT, BufferType::READ_WRITE);
    velocities->clear(glm::vec3(0.0f));
    models_out = Renderer::gen_buffer<Transform>(
        nullptr, sizeof(Transform) * P_COUNT, BufferType::READ_WRITE);

    particles = gen_ref<InstancedMesh>(P_COUNT);  // particles
    particle_material = gen_ref<BasicColorMaterial>(); // particle material
    compute_program =
        gen_ref<ComputeProgram>("./inc/app/Assets/Shaders/NBodyGravity");

    particle_material->set_color(glm::vec4(0.33f, 0.12f, 0.7f, 1.0f));
    particles->set_mesh(gen_scoped<SphereMesh>(2, 0.02f));
    for (int i = 0; i < P_COUNT; i++) {
      glm::vec3 pos = glm::linearRand(glm::vec3(-1.0f), glm::vec3(1.0f));
      particles->push_instance(Transform(glm::translate(glm::mat4(1.0f), pos)));
      (*models_out)[i] = glm::translate(glm::mat4(1.0f), pos);
    }

    G = 0.0002f;
  }
  Referenced<Buffer<Transform>> models_out;
  Referenced<Buffer<glm::vec3>> velocities;
  Referenced<BasicColorMaterial> particle_material;
  Referenced<InstancedMesh> particles;
  Referenced<ComputeProgram> compute_program;
  float G;
  static const int P_COUNT = 128;
};

/**
 * @brief The RenderSystem for the SampleParticleSystem
 */
class SampleParticleSystemRenderer : public RenderSystem<SampleParticleSystem> {
public:
  void render(float dt, Camera *camera,
              SampleParticleSystem *sample_particle_system) override {
    Renderer::wireframe_mode(true);
    auto mesh = sample_particle_system->particles;
    auto material = sample_particle_system->particle_material;
    sample_particle_system->compute_program->barrier(BarrierType::STORAGE);
    mesh->render(camera, material.get(), sample_particle_system);
    Renderer::wireframe_mode(false);
  }
};

/**
 * @brief The PhysicsSystem for the SampleParticleSystem.
 *
 */
class SampleParticleSystemPhysics : public PhysicsSystem<SampleParticleSystem> {
public:
  void update(float dt, SampleParticleSystem *sample_particle_system) override {
    auto program = sample_particle_system->compute_program;
    auto velocities = sample_particle_system->velocities.get();
    auto models_out = sample_particle_system->models_out.get();
    auto particles = sample_particle_system->particles;

    program->bind();
    program->upload_float("dt", dt);
    program->upload_float("G", sample_particle_system->G);

    program->upload_storage("particle_velocities", velocities);
    program->upload_storage("model_instances_out", models_out);
    program->upload_storage("model_instances",
                            particles->get_instance_models());

    program->dispatch_compute(SampleParticleSystem::P_COUNT);

    // swap buffers
    sample_particle_system->models_out =
        particles->swap_instance_models(sample_particle_system->models_out);
  }
};

} // namespace mare

#endif