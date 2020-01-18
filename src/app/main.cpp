#include "mare/Application.hpp"

#include "mare/Meshes/CharMesh.hpp"
#include "mare/Meshes/TriangleMesh.hpp"
#include "mare/Meshes/CircleMesh.hpp"
#include "mare/Meshes/SphereMesh.hpp"
#include "mare/Meshes/TubeMesh.hpp"
#include "mare/Materials/BasicMaterial.hpp"
#include "mare/Materials/PhongMaterial.hpp"
#include "mare/InstancedMesh.hpp"
#include "mare/CompositeMesh.hpp"

// TODO:
// add more primative meshes: Cylinder, Rectangle, Cube, Cone, arrow(from composite mesh), and array mesh
// Abstract Scene interface to create new renderable scenes
// finish input implementation in GLRenderer
// add widgets and UI
// textures and framebuffers
// shadows and lights
// implement pop instance and others in the instanced mesh
// implement instanced mesh of instanced mesh rendering
// Add Text Thickness to CharMesh
// Phong material should render point light not directional light
// organize headers and cpp files, GL files should have protected inheritence from GLRenderer

using namespace mare;

class Sandbox : public mare::Application
{
    glm::vec4 bg_color{0.1f, 0.08f, 0.12f, 1.0f};
    glm::vec4 mesh_color{0.8f, 0.95f, 0.7f, 1.0f};
    SphereMesh* sphere;
    TubeMesh* tube;
    PhongMaterial *phong_mat;
    Camera *main_camera;
    bool wireframe = false;
    InstancedMesh* balls;

    void init(RendererInfo &info) override
    {
        info.name = RendererName::OpenGL;
        info.window_width = 1280;
        info.window_height = 720;
        info.window_aspect = 1280.0f / 720.0f;
        info.major_version = 4;
        info.minor_version = 5;
        info.samples = 16;
        info.fullscreen = false;
        info.vsync = false;
        info.cursor = true;
        info.debug_mode.set(0, 1);
    }

    void startup() override
    {
        set_window_title("Sandbox");

        sphere = new SphereMesh(5, 0.5f);
        sphere->set_scale(glm::vec3(0.25f));

        balls = new InstancedMesh(100);
        balls->set_mesh(sphere);
        balls->push_instance(glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.0f, 0.0f)));
        balls->push_instance(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)));
        balls->push_instance(glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, 0.0f, 0.0f)));

        tube = new TubeMesh(0.0f, 2.0f*3.141592653f, 0.1f, 60);
        tube->set_scale(glm::vec3(0.2f));
        tube->translate({0.0f, 0.3f, 0.0f});

        phong_mat = new PhongMaterial();
        
        main_camera = new Camera(CameraType::PERSPECTIVE);
        main_camera->set_controls(ControlsConfig::ORBITCONTROLS);
        main_camera->set_position(glm::vec3(0.0f, 0.0f, 1.0f));
        set_camera(main_camera);

        enable_depth_testing(true);
        enable_face_culling(true);
    }

    void render(double current_time, double delta_time) override
    {
        clear_color_buffer(bg_color);
        clear_depth_buffer();
        phong_mat->bind();
        phong_mat->render();
        balls->render(phong_mat);
        tube->render(phong_mat);
    }

    void shutdown() override
    {
        // cleanup meshes, materials, and Cameras
        delete sphere;
        delete balls;
        delete tube;
        delete phong_mat;
        delete main_camera;
    }

    void on_key(const RendererInput &input) override
    {
        if (input.W_PRESSED)
        {
            wireframe = !wireframe;
        }
        wireframe_mode(wireframe);
    }
};

int main()
{
    mare::Application *app = new Sandbox();
    app->run(app);
    delete app;
    return 0;
}