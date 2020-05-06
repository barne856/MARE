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
        push_component<SampleParticleSceneControls>();
        push_entity<SampleParticleSystem>();
        set_position(glm::vec3(3.0f));
        look_at(glm::vec3(0.0f));
    }

    void on_enter() override
    {
        // Start with the cursor enabled
        Renderer::API->set_cursor(CURSOR::ENABLED);
    }

    void render(float delta_time) override
    {
        // Renderer properties
        Renderer::API->enable_blending(true);
        Renderer::API->enable_depth_testing(true);
        Renderer::API->enable_face_culling(true);
        // Clear color and depth buffer
        Renderer::API->clear_color_buffer(bg_color);
        Renderer::API->clear_depth_buffer();
    }

    void on_exit() override
    {
        // Show cursor on new scenes
        Renderer::API->set_cursor(CURSOR::ENABLED);
    }

private:
    glm::vec4 bg_color{0.9f, 0.9f, 0.9f, 1.0f};
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