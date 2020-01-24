#ifndef SAMPLEUI1
#define SAMPLEUI1

#include "mare/Application.hpp"
#include "mare/Materials/PhongMaterial.hpp"
#include "mare/Meshes/SlopeMesh.hpp"
#include "mare/Meshes/CubeMesh.hpp"
#include "mare/CompositeMesh.hpp"
#include "mare/InstancedMesh.hpp"

// create UI and run its startup, render and shutdown in here

namespace mare
{
class SampleUI1 : public Layer
{
public:
    SampleUI1()
    {
        // Create the camera and controls
        main_camera = new Camera(CameraType::PERSPECTIVE);
        main_camera->set_controls(ControlsConfig::ORBITCONTROLS);
        main_camera->set_position(glm::vec3(0.0f, 0.0f, 1.0f));
        Application::set_camera(main_camera);

        // Create the letter M
        const float SQRT2 = 1.41421356237f;
        const float PI = 3.141592653f;
        cube = new CubeMesh(1.0f);
        cubes = new InstancedMesh(5);
        cubes->set_mesh(cube);
        cubes->push_instance( glm::translate(glm::mat4(1.0f), {0.0f, 4.0f - 1.5f * SQRT2, 0.0f}) * glm::rotate(glm::mat4(1.0f), PI / 4.0f, {0.0f, 0.0f, 1.0f}) );
        cubes->push_instance(glm::translate(glm::mat4(1.0f), {-0.5f * SQRT2, 4.0f - SQRT2, 0.0f}) * glm::rotate(glm::mat4(1.0f), PI / 4.0f, {0.0f, 0.0f, 1.0f}));
        cubes->push_instance(glm::translate(glm::mat4(1.0f), {0.5f * SQRT2, 4.0f - SQRT2, 0.0f}) * glm::rotate(glm::mat4(1.0f), PI / 4.0f, {0.0f, 0.0f, 1.0f}));
        cubes->push_instance(glm::translate(glm::mat4(1.0f), {-SQRT2-0.5f, 2.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), {1.0f, 4.0f, 1.0f}));
        cubes->push_instance(glm::translate(glm::mat4(1.0f), {SQRT2+0.5f, 2.0f, 0.0f}) * glm::scale(glm::mat4(1.0f), {1.0f, 4.0f, 1.0f}));
        slope = new SlopeMesh(1.0f);
        slopes = new InstancedMesh(2);
        slopes->set_mesh(slope);
        slopes->push_instance(glm::translate(glm::mat4(1.0f), {-SQRT2, 4.0f - 0.5f * SQRT2, 0.0f}) * glm::rotate(glm::mat4(1.0f), PI / 4.0f, {0.0f, 0.0f, 1.0f}));
        slopes->push_instance(glm::translate(glm::mat4(1.0f), {SQRT2, 4.0f - 0.5f * SQRT2, 0.0f}) * glm::rotate(glm::mat4(1.0f), 5.0f * PI / 4.0f, {0.0f, 0.0f, 1.0f}));
        letter_M = new CompositeMesh();
        letter_M->push_mesh(cubes);
        letter_M->push_mesh(slopes);
        letter_M->set_scale(glm::vec3(0.05f));
        letter_M->rotate({1.0f, 0.0f, 0.0f}, PI/2.0f);

        // Create the material
        phong_mat = new PhongMaterial();
        phong_mat->bind();

        // Renderer properties
        Application::enable_depth_testing(true);
        Application::enable_face_culling(true);
    }

    bool render(double time, double dt) override
    {
        // Clear color and depth buffer
        Application::clear_color_buffer(bg_color);
        Application::clear_depth_buffer();
        
        // Set camera
        Application::set_camera(main_camera);

        // Render the letter M
        phong_mat->render();
        letter_M->render(phong_mat);

        // Run forever
        return true;
    }

    ~SampleUI1()
    {
        // cleanup meshes, materials, and Cameras
        delete cubes;
        delete slopes;
        delete cube;
        delete slope;
        delete letter_M;
        delete phong_mat;
        delete main_camera;
    }

private:
    glm::vec4 bg_color{0.1f, 0.08f, 0.12f, 1.0f};
    glm::vec4 mesh_color{0.8f, 0.95f, 0.7f, 1.0f};

    CompositeMesh *letter_M;
    Mesh *cube;
    Mesh *slope;
    InstancedMesh *slopes;
    InstancedMesh *cubes;

    PhongMaterial *phong_mat;
    Camera *main_camera;
};
} // namespace mare

#endif