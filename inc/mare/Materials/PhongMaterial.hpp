#ifndef PHONGMATERIAL
#define PHONGMATERIAL

// External Libraries
#include "glm.hpp"

// MARE
#include "mare/Material.hpp"

namespace mare
{
class PhongMaterial : public virtual Material
{
public:
    PhongMaterial() : Material("./res/Shaders/Phong")
    {
        material.ambient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        material.diffuse = glm::vec4(0.96f, 0.17f, 0.71f, 1.0f);
        material.specular = glm::vec4(1.0f);
        material.shininess = 32.0f;
        light.ambient = glm::vec4(1.0f);
        light.diffuse = glm::vec4(1.0f);
        light.specular = glm::vec4(1.0f);
        light_position = glm::vec4(0.0f, 0.6f, 0.0f, 0.0f);
        bind_buffer_block("material_properties", sizeof(material));
        bind_buffer_block("light_properties", sizeof(light));
    }
    virtual ~PhongMaterial() {}
    void render() override
    {
        upload_buffer_block("material_properties", &material);
        upload_buffer_block("light_properties", &light);
        upload_vec4("light_position", light_position);
    }

protected:
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
    glm::vec4 light_position;
};
} // namespace mare

#endif