#ifndef MARETEXTOBJECT
#define MARETEXTOBJECT

// MARE
#include "mare/Object.hpp"
#include "mare/Materials/PhongMaterial.hpp"
#include "mare/Meshes/SlopeMesh.hpp"
#include "mare/Meshes/CubeMesh.hpp"
#include "mare/CompositeMesh.hpp"
#include "mare/InstancedMesh.hpp"

namespace mare
{
class MareTextObject : public Object
{
public:
    MareTextObject()
    {
        // Create the letter M
        const float SQRT2 = 1.41421356237f;
        const float PI = 3.141592653f;
        cube_mesh = Renderer::API->GenScoped<CubeMesh>(1.0f);
        cubes_mesh = Renderer::API->GenScoped<InstancedMesh>(5);
        cubes_mesh->set_mesh(std::move(cube_mesh));
        cubes_mesh->push_instance(glm::translate(glm::mat4(1.0f), {0.0f, 4.0f - 1.5f * SQRT2, 0.0f}) * glm::rotate(glm::mat4(1.0f), PI / 4.0f, {0.0f, 0.0f, 1.0f}));
        cubes_mesh->push_instance(glm::translate(glm::mat4(1.0f), {-0.5f * SQRT2, 4.0f - SQRT2, 0.0f}) * glm::rotate(glm::mat4(1.0f), PI / 4.0f, {0.0f, 0.0f, 1.0f}));
        cubes_mesh->push_instance(glm::translate(glm::mat4(1.0f), {0.5f * SQRT2, 4.0f - SQRT2, 0.0f}) * glm::rotate(glm::mat4(1.0f), PI / 4.0f, {0.0f, 0.0f, 1.0f}));
        cubes_mesh->push_instance(glm::translate(glm::mat4(1.0f), {-SQRT2 - 0.5f, 2.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), {1.0f, 4.0f, 1.0f}));
        cubes_mesh->push_instance(glm::translate(glm::mat4(1.0f), {SQRT2 + 0.5f, 2.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), {1.0f, 4.0f, 1.0f}));
        slope_mesh = Renderer::API->GenScoped<SlopeMesh>(1.0f);
        slopes_mesh = Renderer::API->GenScoped<InstancedMesh>(2);
        slopes_mesh->set_mesh(std::move(slope_mesh));
        slopes_mesh->push_instance(glm::translate(glm::mat4(1.0f), {-SQRT2, 4.0f - 0.5f * SQRT2, 0.0f}) * glm::rotate(glm::mat4(1.0f), PI / 4.0f, {0.0f, 0.0f, 1.0f}));
        slopes_mesh->push_instance(glm::translate(glm::mat4(1.0f), {SQRT2, 4.0f - 0.5f * SQRT2, 0.0f}) * glm::rotate(glm::mat4(1.0f), 5.0f * PI / 4.0f, {0.0f, 0.0f, 1.0f}));
        letter_M_mesh = Renderer::API->GenScoped<CompositeMesh>();
        letter_M_mesh->push_mesh(std::move(cubes_mesh));
        letter_M_mesh->push_mesh(std::move(slopes_mesh));
        letter_M_mesh->set_scale(glm::vec3(0.05f));
        letter_M_mesh->rotate({1.0f, 0.0f, 0.0f}, PI / 2.0f);

        // Create the materials
        phong_mat = Renderer::API->GenScoped<PhongMaterial>();

        // push the letter M as a primative
        push_primative(std::move(letter_M_mesh), std::move(phong_mat));
    }

    void set_light_intensity(float intensity)
    {
        phong_mat->light.ambient = glm::vec4(glm::vec3(intensity), 1.0f);
        phong_mat->light.diffuse = glm::vec4(glm::vec3(intensity), 1.0f);
        phong_mat->light.specular = glm::vec4(glm::vec3(intensity), 1.0f);
    }

private:
    glm::vec4 mesh_color{0.8f, 0.95f, 0.7f, 1.0f};

    Scoped<CompositeMesh> letter_M_mesh;
    Scoped<Mesh> cube_mesh;
    Scoped<Mesh> slope_mesh;
    Scoped<InstancedMesh> slopes_mesh;
    Scoped<InstancedMesh> cubes_mesh;
    Scoped<Object> letter_M;

    Scoped<PhongMaterial> phong_mat;
};
} // namespace mare

#endif