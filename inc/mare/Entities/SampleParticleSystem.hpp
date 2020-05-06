#ifndef SAMPLEPARTICLESYSTEM
#define SAMPLEPARTICLESYSTEM

// MARE
#include "mare/Systems.hpp"
#include "mare/Buffers.hpp"
#include "mare/Renderer.hpp"
#include "mare/Entity.hpp"
#include "mare/Mare.hpp"
#include "mare/Meshes.hpp"
#include "mare/Shader.hpp"

#include "mare/Materials/BasicMaterial.hpp"
#include "mare/Meshes/SphereMesh.hpp"

#include "gtc/random.hpp"

static const int P_COUNT = 1024;

namespace mare
{

    // forward declare components
    class SampleParticleSystemRenderer;
    class SampleParticleSystemPhysics;

    class SampleParticleSystem : public Entity
    {
    public:
        SampleParticleSystem()
        {
            push_component<SampleParticleSystemRenderer>();
            push_component<SampleParticleSystemPhysics>();
            velocities = Renderer::API->GenBuffer<glm::vec3>(nullptr, sizeof(glm::vec3) * P_COUNT, BufferType::READ_WRITE);
            velocities_out = Renderer::API->GenBuffer<glm::vec3>(nullptr, sizeof(glm::vec3) * P_COUNT, BufferType::READ_WRITE);
            models_out = Renderer::API->GenBuffer<glm::mat4>(nullptr, sizeof(glm::mat4) * P_COUNT, BufferType::READ_WRITE);

            push_asset<InstancedMesh>(P_COUNT); // particles
            push_asset<BasicMaterial>();        // particle material
            push_asset<ComputeProgram>("./res/Shaders/NBodyGravity");

            get_asset<BasicMaterial>()->set_color(glm::vec4(0.33f, 0.12f, 0.7f, 1.0f));
            std::vector<glm::vec3> zeros(P_COUNT, glm::vec3(0.0f));
            auto particles = get_asset<InstancedMesh>();
            //particles->set_mesh(Renderer::API->GenScoped<CircleMesh>(8, 0.006f));
            particles->set_mesh(Renderer::API->GenScoped<SphereMesh>(2, 0.01f));
            for (int i = 0; i < P_COUNT; i++)
            {
                glm::vec3 pos = glm::linearRand(glm::vec3(-1.0f), glm::vec3(1.0f));
                //pos.z = 0.0f;
                particles->push_instance(glm::translate(glm::mat4(1.0f), pos));
                glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
                (*models_out.get())[i] = model;
                (*velocities.get())[i] = 0.0f*glm::vec3(-pos.y, pos.x, pos.z);
                (*velocities_out.get())[i] = 0.0f*glm::vec3(-pos.y, pos.x, pos.z);
            }

            G = 0.0001f;
        }
        Referenced<Buffer<glm::mat4>> models_out;
        Referenced<Buffer<glm::vec3>> velocities;
        Referenced<Buffer<glm::vec3>> velocities_out;
        float G;
    };

    class SampleParticleSystemRenderer : public RenderSystem<SampleParticleSystem>
    {
    public:
        void render(SampleParticleSystem *sample_particle_system, Camera *camera, float dt) override
        {
            Renderer::API->wireframe_mode(true);
            auto mesh = sample_particle_system->get_asset<InstancedMesh>();
            auto material = sample_particle_system->get_asset<BasicMaterial>();
            mesh->render(camera, material, sample_particle_system->get_model());
            Renderer::API->wireframe_mode(false);
        }
    };

    class SampleParticleSystemPhysics : public PhysicsSystem<SampleParticleSystem>
    {
    public:
        void update(SampleParticleSystem *sample_particle_system, float dt) override
        {
            auto program = sample_particle_system->get_asset<ComputeProgram>();
            auto velocities = sample_particle_system->velocities;
            auto velocities_out = sample_particle_system->velocities_out;
            auto models_out = sample_particle_system->models_out;
            auto particles = sample_particle_system->get_asset<InstancedMesh>();
            program->bind();
            // swap buffers
            if (frame % 2)
            {
                program->upload_storage("particle_velocities", velocities.get());
                program->upload_storage("particle_velocities_out", velocities_out.get());
                program->upload_storage("model_instances_out", models_out.get());
                program->upload_storage("model_instances", particles->get_instance_models());
            }
            else
            {
                program->upload_storage("particle_velocities_out", velocities.get());
                program->upload_storage("particle_velocities", velocities_out.get());
                program->upload_storage("model_instances", models_out.get());
                program->upload_storage("model_instances_out", particles->get_instance_models());
            }

            program->upload_float("dt", dt);
            program->upload_float("G", sample_particle_system->G);
            program->dispatch_compute(P_COUNT);

            frame++;
        }

    private:
        int frame = 0;
    };

} // namespace mare

#endif