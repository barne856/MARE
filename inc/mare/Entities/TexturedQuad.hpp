#ifndef TEXTUREDQUAD
#define TEXTUREDQUAD

// MARE
#include "mare/Materials/BasicTextureMaterial.hpp"
#include "mare/Systems.hpp"
#include "mare/Renderer.hpp"
#include "mare/Entity.hpp"
#include "mare/Meshes/QuadrangleMesh.hpp"
#include "mare/Mare.hpp"

namespace mare
{

    // forward declare SampleTorus components
    class TexturedQuadRenderer;

    class TexturedQuad : public Entity
    {
    public:
        TexturedQuad()
        {
            gen_asset<QuadrangleMesh>();
            gen_asset<BasicTextureMaterial>();
            gen_component<TexturedQuadRenderer>();
        }
        TexturedQuad(const char *filepath)
        {
            gen_asset<QuadrangleMesh>();
            gen_asset<BasicTextureMaterial>();
            gen_component<TexturedQuadRenderer>();
            texture_ = Renderer::gen_texture2D(filepath);
            auto material = get_asset<BasicTextureMaterial>();
            material->set_texture(texture_);
        }
        void set_texture(Referenced<Texture2D> texture)
        {
            auto material = get_asset<BasicTextureMaterial>();
            texture_ = texture;
            material->set_texture(texture_);
        }

    private:
        Referenced<Texture2D> texture_;
    };

    class TexturedQuadRenderer : public RenderSystem<TexturedQuad>
    {
    public:
        void render(TexturedQuad *textured_quad, Camera *camera, float dt) override
        {
            auto meshes = textured_quad->get_assets<Mesh>();
            auto materials = textured_quad->get_assets<Material>();
            meshes[0]->render(camera, materials[0], textured_quad->get_model());
        }
    };

} // namespace mare

#endif