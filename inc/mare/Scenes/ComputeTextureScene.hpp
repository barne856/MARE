#ifndef COMPUTETEXTURESCENE
#define COMPUTETEXTURESCENE

// MARE
#include "mare/Renderer.hpp"
#include "mare/Scene.hpp"
#include "mare/Entities/TexturedQuad.hpp"

namespace mare
{

class ComputeTextureSceneControls;

class ComputeTextureScene : public Scene
{
public:
    ComputeTextureScene() : Scene(ProjectionType::ORTHOGRAPHIC)
    {
        push_component<ComputeTextureSceneControls>();
        push_entity(Renderer::API->GenScoped<TexturedQuad>("./res/Textures/checkerboard.png"));
    }

    void on_enter() override
    {
        // Start with the cursor disabled
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

class ComputeTextureSceneControls : public ControlsSystem<ComputeTextureScene>
{
public:
    bool on_key(ComputeTextureScene *scene, const RendererInput &input) override
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