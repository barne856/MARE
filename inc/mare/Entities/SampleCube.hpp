#ifndef SAMPLECUBE
#define SAMPLECUBE

// MARE
#include "mare/Meshes/TorusMesh.hpp"
#include "mare/Materials/PhongMaterial.hpp"
#include "mare/Systems.hpp"
#include "mare/Renderer.hpp"
#include "mare/Entity.hpp"
#include "mare/Meshes/CubeMesh.hpp"
#include "mare/Components/Rendering/ShadowMap.hpp"

namespace mare
{

    // forward declare SampleTorus components
    class SampleCubeRenderer;

    class SampleCube : public Entity
    {
    public:
        SampleCube()
        {
            push_asset<CubeMesh>(1.0f);
            push_asset<PhongMaterial>();
            push_component<SampleCubeRenderer>();
        }
    };

    class SampleCubeRenderer : public RenderSystem<SampleCube>
    {
    public:
        void render(SampleCube *sample_cube, Camera *camera, float dt) override
        {
            auto meshes = sample_cube->get_assets<Mesh>();
            auto materials = sample_cube->get_assets<Material>();
            ShadowMap *smc = camera->get_component<ShadowMap>();
            if (smc)
            {
                materials[0]->bind();
                glm::mat4 shadow_matrix = smc->scale_bias_matrix * smc->light_view->projection() * smc->light_view->view();
                materials[0]->upload_mat4("shadow_matrix", shadow_matrix);
                materials[0]->upload_texture2D("depth_texture", smc->depth_buffer->depth_texture());
            }
            meshes[0]->render(camera, materials[0], sample_cube->get_model());
        }
    };

} // namespace mare

#endif