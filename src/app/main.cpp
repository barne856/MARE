#include "mare/Application.hpp"

#include "mare/Meshes/CharMesh.hpp"
#include "mare/Meshes/TriangleMesh.hpp"
#include "mare/Meshes/CircleMesh.hpp"
#include "mare/Meshes/CubeMesh.hpp"
#include "mare/Meshes/ConeMesh.hpp"
#include "mare/Meshes/CylinderMesh.hpp"
#include "mare/Meshes/SphereMesh.hpp"
#include "mare/Meshes/TubeMesh.hpp"
#include "mare/Materials/BasicMaterial.hpp"
#include "mare/Materials/PhongMaterial.hpp"
#include "mare/InstancedMesh.hpp"
#include "mare/CompositeMesh.hpp"

// TODO:
// render state should own and delete buffers, fix arraymesh
// add more primative meshes: Slope, Torus, arrow(from composite mesh), and array mesh
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
// Cylinder, Cone, and Tube meshes should have an option for flat shading normals or smooth shading normals
// add exception handling

using namespace mare;

class Sandbox : public mare::Application
{
    glm::vec4 bg_color{0.1f, 0.08f, 0.12f, 1.0f};
    glm::vec4 mesh_color{0.8f, 0.95f, 0.7f, 1.0f};
    ConeMesh* cone;
    InstancedMesh* inst_cones;
    PhongMaterial *phong_mat;
    Camera *main_camera;
    bool wireframe = false;

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

        cone = new ConeMesh(0.2f, 60);
        cone->translate({0.0f, 0.0f, 0.05f});

        inst_cones = new InstancedMesh(5);
        inst_cones->set_mesh(cone);
        inst_cones->push_instance(glm::translate(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f}));
        inst_cones->push_instance(glm::translate(glm::mat4(1.0f), {0.5f, 0.0f, 0.0f}));
        inst_cones->push_instance(glm::translate(glm::mat4(1.0f), {-0.5f, 0.0f, 0.0f}));
        inst_cones->push_instance(glm::translate(glm::mat4(1.0f), {0.0f, 0.5f, 0.0f}));
        inst_cones->push_instance(glm::translate(glm::mat4(1.0f), {0.0f, -0.5f, 0.0f}));

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
        inst_cones->render(phong_mat);
    }

    void shutdown() override
    {
        // cleanup meshes, materials, and Cameras
        delete cone;
        delete inst_cones;
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