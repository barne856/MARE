#ifndef PHONGMATERIAL
#define PHONGMATERIAL

// External Libraries
#include "glm.hpp"

// MARE
#include "mare/Shader.hpp"
#include "mare/Buffers.hpp"
#include "mare/Renderer.hpp"

namespace mare
{

struct phong_properties
{
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
    float shininess;
};

struct phong_light
{
    glm::vec4 ambient;
    glm::vec4 diffuse;
    glm::vec4 specular;
};

class PhongMaterial : public virtual Material
{
public:
    PhongMaterial() : Material("./res/Shaders/Phong")
    {
        texture = Renderer::API->GenTexture2DBuffer("./res/Textures/weave_diffuse.jpg");
        phong_properties props = {glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                  glm::vec4(0.1f, 0.1f, 0.5f, 1.0f),
                                  glm::vec4(1.0f),
                                  32.0f};
        phong_light lit = {glm::vec4(1.0f),
                           glm::vec4(1.0f),
                           glm::vec4(1.0f)};
        light_position = glm::vec3(0.0f, 1.0f, 1.0f);
        properties = Renderer::API->GenBuffer<phong_properties>(&props, sizeof(phong_properties));
        properties->set_format({{Attribute::UNIFORM, "material_properties"}});
        light = Renderer::API->GenBuffer<phong_light>(&lit, sizeof(phong_light));
        light->set_format({{Attribute::UNIFORM, "light_properties"}});
    }
    virtual ~PhongMaterial() {}
    void render() override
    {
        upload_texture2D("tex", texture.get());
        upload_uniform("material_properties", properties.get());
        upload_uniform("light_properties", light.get());
        upload_vec3("light_position", light_position);
    }
    inline glm::vec3 get_light_position() const
    {
        return light_position;
    }
    glm::vec3 light_position;

private:
    Scoped<Texture2DBuffer> texture;
    Scoped<Buffer<phong_properties>> properties;
    Scoped<Buffer<phong_light>> light;
};
} // namespace mare

#endif