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
            push_asset<QuadrangleMesh>();
            push_asset<BasicTextureMaterial>();
            push_component<TexturedQuadRenderer>();
        }
        TexturedQuad(const char *filepath)
        {
            push_asset<QuadrangleMesh>();
            push_asset<BasicTextureMaterial>();
            push_component<TexturedQuadRenderer>();
            texture_ = Renderer::API->GenTexture2DBuffer(filepath);
            auto material = get_asset<BasicTextureMaterial>();
            material->set_texture(texture_);
        }
        void set_texture(Referenced<Texture2DBuffer> texture)
        {
            auto material = get_asset<BasicTextureMaterial>();
            texture_ = texture;
            material->set_texture(texture_);
        }

    private:
        Referenced<Texture2DBuffer> texture_;
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