#ifndef SHADOWMAP
#define SHADOWMAP

// MARE
#include "mare/Mare.hpp"
#include "mare/Meshes.hpp"
#include "mare/Entities/Camera.hpp"
#include "mare/Systems.hpp"
#include "mare/Scene.hpp"
#include "mare/Renderer.hpp"
#include "mare/Materials/BasicMaterial.hpp"

// External Libraries
#include "glm.hpp"

// OpenGL
#include "GL/glew.h"

namespace mare
{

    class ShadowMap : public RenderSystem<Scene>
    {
    public:
        ShadowMap(int oversample = 1) : oversample_(oversample)
        {
            scale_bias_matrix = glm::mat4(glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
                                          glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
                                          glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
                                          glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
            depth_buffer = Renderer::API->GenFramebuffer(oversample_ * Renderer::get_info().window_width, oversample_ * Renderer::get_info().window_height);
            light_view = Renderer::API->GenScoped<Camera>(ProjectionType::PERSPECTIVE);
            material = Renderer::API->GenScoped<BasicMaterial>();
            light_pos = glm::vec3(5.0f, 0.0f, 5.0f);
            light_center = glm::vec3(0.0f);
            light_view->set_position(light_pos);
            light_view->look_at(light_center);
        }
        ~ShadowMap() {}
        void render(Scene *scene, Camera *camera, float dt) override
        {

            // Render all meshes from all entities in the scene from the perspective of the light into the depth buffer
            Renderer::API->set_framebuffer(depth_buffer.get());

            // Renderer properties
            Renderer::API->enable_depth_testing(true);
            Renderer::API->enable_face_culling(false);
            // Clear depth buffer
            Renderer::API->clear_depth_buffer();

            glViewport(0, 0, oversample_ * Renderer::API->get_info().window_width, oversample_ * Renderer::API->get_info().window_height);
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(4.0f, 4.0f);
            for (auto entity_it = scene->entity_begin(); entity_it != scene->entity_end(); entity_it++)
            {
                Entity *entity = entity_it->get();
                auto meshes = entity->get_assets<Mesh>();
                for (auto mesh : meshes)
                {
                    // render from perspective of the light
                    mesh->render(light_view.get(), material.get(), entity->get_model());
                }
            }
            // return to default framebuffer
            glDisable(GL_POLYGON_OFFSET_FILL);
            Renderer::API->set_framebuffer(nullptr);
            glViewport(0, 0, Renderer::API->get_info().window_width, Renderer::API->get_info().window_height);
            // Render entities normally with a shader that draws shadows...
        }
        Scoped<Framebuffer> depth_buffer;
        Scoped<Camera> light_view;
        glm::mat4 scale_bias_matrix;
        Scoped<BasicMaterial> material;
        glm::vec3 light_pos;
        glm::vec3 light_center;

    private:
        int oversample_;
    };
} // namespace mare

#endif