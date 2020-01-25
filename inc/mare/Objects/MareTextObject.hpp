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
        cube_mesh = new CubeMesh(1.0f);
        cubes_mesh = new InstancedMesh(5);
        cubes_mesh->set_mesh(cube_mesh);
        cubes_mesh->push_instance(glm::translate(glm::mat4(1.0f), {0.0f, 4.0f - 1.5f * SQRT2, 0.0f}) * glm::rotate(glm::mat4(1.0f), PI / 4.0f, {0.0f, 0.0f, 1.0f}));
        cubes_mesh->push_instance(glm::translate(glm::mat4(1.0f), {-0.5f * SQRT2, 4.0f - SQRT2, 0.0f}) * glm::rotate(glm::mat4(1.0f), PI / 4.0f, {0.0f, 0.0f, 1.0f}));
        cubes_mesh->push_instance(glm::translate(glm::mat4(1.0f), {0.5f * SQRT2, 4.0f - SQRT2, 0.0f}) * glm::rotate(glm::mat4(1.0f), PI / 4.0f, {0.0f, 0.0f, 1.0f}));
        cubes_mesh->push_instance(glm::translate(glm::mat4(1.0f), {-SQRT2 - 0.5f, 2.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), {1.0f, 4.0f, 1.0f}));
        cubes_mesh->push_instance(glm::translate(glm::mat4(1.0f), {SQRT2 + 0.5f, 2.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), {1.0f, 4.0f, 1.0f}));
        slope_mesh = new SlopeMesh(1.0f);
        slopes_mesh = new InstancedMesh(2);
        slopes_mesh->set_mesh(slope_mesh);
        slopes_mesh->push_instance(glm::translate(glm::mat4(1.0f), {-SQRT2, 4.0f - 0.5f * SQRT2, 0.0f}) * glm::rotate(glm::mat4(1.0f), PI / 4.0f, {0.0f, 0.0f, 1.0f}));
        slopes_mesh->push_instance(glm::translate(glm::mat4(1.0f), {SQRT2, 4.0f - 0.5f * SQRT2, 0.0f}) * glm::rotate(glm::mat4(1.0f), 5.0f * PI / 4.0f, {0.0f, 0.0f, 1.0f}));
        letter_M_mesh = new CompositeMesh();
        letter_M_mesh->push_mesh(cubes_mesh);
        letter_M_mesh->push_mesh(slopes_mesh);
        letter_M_mesh->set_scale(glm::vec3(0.05f));
        letter_M_mesh->rotate({1.0f, 0.0f, 0.0f}, PI / 2.0f);

        // Create the materials
        phong_mat = new PhongMaterial();

        // push the letter M as a primative
        push_primative(letter_M_mesh, phong_mat);
    }

    void set_light_intensity(float intensity)
    {
        phong_mat->light.ambient = glm::vec4(glm::vec3(intensity), 1.0f);
        phong_mat->light.diffuse = glm::vec4(glm::vec3(intensity), 1.0f);
        phong_mat->light.specular = glm::vec4(glm::vec3(intensity), 1.0f);
    }

private:
    glm::vec4 mesh_color{0.8f, 0.95f, 0.7f, 1.0f};

    CompositeMesh *letter_M_mesh;
    Mesh *cube_mesh;
    Mesh *slope_mesh;
    InstancedMesh *slopes_mesh;
    InstancedMesh *cubes_mesh;
    Object *letter_M;

    PhongMaterial *phong_mat;
};
} // namespace mare

#endif