#ifndef COMPUTETEXTURESCENE
#define COMPUTETEXTURESCENE

// MARE
#include "mare/Renderer.hpp"
#include "mare/Scene.hpp"
#include "mare/Entities/TexturedQuad.hpp"
#include "mare/Shader.hpp"

namespace mare
{

class ComputeTextureSceneControls;

class ComputeTextureScene : public Scene
{
public:
    ComputeTextureScene() : Scene(ProjectionType::ORTHOGRAPHIC)
    {
        push_component<ComputeTextureSceneControls>();
        push_entity<TexturedQuad>();
        auto tex_quad = get_entity<TexturedQuad>();
        
        // create empty texture
        texture = Renderer::API->GenTexture2DBuffer(TextureType::RGBA32F, 16*32, 16*32);
        //texture = Renderer::API->GenTexture2DBuffer("./res/Textures/checkerboard.png");
        // upload texture to comp shader
        sample_compute = Renderer::API->GenRef<ComputeProgram>("./res/Shaders/SampleCompute");
        sample_compute->bind();
        sample_compute->upload_image2D("data", texture.get());
        // run compute program
        sample_compute->compute(32, 32, 1);
        // upload texture to texturedQuad
        tex_quad->set_texture(texture);
        tex_quad->set_scale(glm::vec3(2.0f*aspect(), 2.0f, 1.0f));
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
    Referenced<Texture2DBuffer> texture;
    Referenced<ComputeProgram> sample_compute;
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