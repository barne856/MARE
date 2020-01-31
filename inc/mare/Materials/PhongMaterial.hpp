#ifndef PHONGMATERIAL
#define PHONGMATERIAL

// External Libraries
#include "glm.hpp"

// MARE
#include "mare/Material.hpp"
#include "mare/Texture.hpp"
#include "mare/Application.hpp"

namespace mare
{
class PhongMaterial : public virtual Material
{
public:
    PhongMaterial() : Material("./res/Shaders/Phong")
    {
        texture = Application::GenTexture2D("./res/Textures/checkerboard.png");
        material.ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        material.diffuse = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        material.specular = glm::vec4(1.0f);
        material.shininess = 32.0f;
        light.ambient = glm::vec4(1.0f);
        light.diffuse = glm::vec4(1.0f);
        light.specular = glm::vec4(1.0f);
        light_position = glm::vec3(0.0f, 1.0f, 1.0f);
        bind_buffer_block("material_properties", sizeof(material));
        bind_buffer_block("light_properties", sizeof(light));
    }
    virtual ~PhongMaterial() {}
    void render() override
    {
        texture->bind(0);
        upload_buffer_block("material_properties", &material);
        upload_buffer_block("light_properties", &light);
        upload_vec3("light_position", light_position);
    }
    inline glm::vec3 get_light_position() const
    {
        return light_position;
    }

    struct
    {
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float shininess;
    } material;
    struct
    {
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
    } light;
    glm::vec3 light_position;
    private:
    Texture2D* texture;
};
} // namespace mare

#endif