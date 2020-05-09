#ifndef SAMPLEPARTICLESCENE
#define SAMPLEPARTICLESCENE

// MARE
#include "mare/Renderer.hpp"
#include "mare/Scene.hpp"
#include "mare/Shader.hpp"
#include "mare/Entities/SampleParticleSystem.hpp"

namespace mare
{

    class SampleParticleSceneControls;

    class SampleParticleScene : public Scene
    {
    public:
        SampleParticleScene() : Scene(ProjectionType::PERSPECTIVE)
        {
            gen_component<SampleParticleSceneControls>();
            gen_entity<SampleParticleSystem>();
            t = 0.0f;
            r = 2.0f;
            float x = r * cos(t);
            float y = r * sin(t);
            float z = r * sin(t);
            set_position(glm::vec3(x, y, z));
            look_at(glm::vec3(0.0f));
        }

        void on_enter() override
        {
            // Start with the cursor enabled
            Renderer::set_cursor(CURSOR::ENABLED);
        }

        void render(float delta_time) override
        {
            // Renderer properties
            Renderer::enable_blending(true);
            Renderer::enable_depth_testing(true);
            Renderer::enable_face_culling(false);
            // Clear color and depth buffer
            Renderer::clear_color_buffer(bg_color);
            Renderer::clear_depth_buffer();

            // rotate view
            t += delta_time/10.0f;
            float x = r * cos(t);
            float y = r * sin(t);
            float z = r * sin(t);
            set_position(glm::vec3(x, y, z));
            look_at(glm::vec3(0.0f));
        }

        void on_exit() override
        {
            // Show cursor on new scenes
            Renderer::set_cursor(CURSOR::ENABLED);
        }

    private:
        glm::vec4 bg_color{0.9f, 0.9f, 0.9f, 1.0f};
        float t;
        float r;
    };

    class SampleParticleSceneControls : public ControlsSystem<SampleParticleScene>
    {
    public:
        bool on_key(SampleParticleScene *scene, const RendererInput &input) override
        {
            if (input.ESCAPE_JUST_PRESSED)
            {
                Renderer::end_renderer();
            }
            // event is handled
            return true;
        }
    };

} // namespace mare

#endif