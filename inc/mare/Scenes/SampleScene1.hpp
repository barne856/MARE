#ifndef SAMPLESCENE1
#define SAMPLESCENE1

#include "mare/Application.hpp"
#include "mare/Materials/PhongMaterial.hpp"
#include "mare/Meshes/SlopeMesh.hpp"
#include "mare/Meshes/CubeMesh.hpp"
#include "mare/CompositeMesh.hpp"
#include "mare/InstancedMesh.hpp"
#include "mare/Layer.hpp"
#include "mare/Object.hpp"

// create UI and run its startup, render and shutdown in here

namespace mare
{
class SampleScene1 : public Layer
{
public:
    SampleScene1()
    {
        // Create the camera and controls
        set_camera(new Camera(CameraType::PERSPECTIVE));
        get_camera()->set_controls(ControlsConfig::ORBITCONTROLS);
        get_camera()->set_position(glm::vec3(0.0f, 0.0f, 1.0f));

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

        // Create the material
        phong_mat = new PhongMaterial();

        letter_M = new Object();
        letter_M->push_primative(letter_M_mesh, phong_mat);
    }

    bool render(double time, double dt) override
    {
        // Renderer properties
        Application::enable_depth_testing(true);
        Application::enable_face_culling(true);

        // Render the letter M
        letter_M->render(this);

        // Run forever
        return true;
    }

    ~SampleScene1()
    {
        // cleanup (deleting an object deletes all meshes and materials used to render it)
        delete letter_M;
    }

    bool on_key(const RendererInput &input) override
    {
        if (input.W_PRESSED)
        {
            wireframe = !wireframe;
        }
        Application::wireframe_mode(wireframe);
        // event is handled
        return true;
    }

    bool on_mouse_button(const RendererInput &input) override
    {
        if (input.mouse_button == 1)
        {
            Application::set_focus(this);
            return true;
        }
        Application::set_focus(nullptr);
        return false;
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

    bool wireframe = false;
};
} // namespace mare

#endif