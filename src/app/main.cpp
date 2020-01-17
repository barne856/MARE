#include "mare/Application.hpp"

#include "mare/Meshes/CharMesh.hpp"
#include "mare/Meshes/TriangleMesh.hpp"
#include "mare/Materials/BasicMaterial.hpp"
#include "mare/InstancedMesh.hpp"
#include "mare/CompositeMesh.hpp"

// TODO:
// organize headers and cpp files, GL files should have protected inheritence from GLRenderer
// add more primative meshes, array mesh, and Text mesh
// Abstract Scene interface to create new renderable scenes
// finish input implementation in GLRenderer
// add widgets and UI
// textures and framebuffers
// shadows and lights
// implement pop instance and others in the instanced mesh
// implement instanced mesh or instanced mesh rendering
// Add Text Thickness to CharMesh

using namespace mare;

class Sandbox : public mare::Application
{
    glm::vec4 bg_color{0.1f, 0.08f, 0.12f, 1.0f};
    glm::vec4 tri_color{0.8f, 0.95f, 0.7f, 1.0f};
    CharMesh *text;
    TriangleMesh* tri;
    InstancedMesh *instanced_text;
    CompositeMesh* comp_mesh;
    BasicMaterial *basic_mat;
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

        text = new CharMesh("M.A.R.E.");
        text->set_scale(glm::vec3(0.15f, 0.3f, 0.3f));
        text->set_center(glm::vec3(0.0f));

        tri = new TriangleMesh({-0.25f, -0.25f}, {0.25f, -0.25f}, {0.25f, 0.25f});

        instanced_text = new InstancedMesh(3);
        instanced_text->set_mesh(text);
        instanced_text->push_instance(glm::mat4(1.0f));
        instanced_text->push_instance(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.3f, 0.0f)));
        instanced_text->push_instance(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.3f, 0.0f)));

        comp_mesh = new CompositeMesh();
        comp_mesh->push_mesh(instanced_text);
        comp_mesh->push_mesh(tri);

        basic_mat = new BasicMaterial();
        basic_mat->bind();
        basic_mat->upload_vec4("u_color", tri_color);

        main_camera = new Camera(CameraType::PERSPECTIVE);
        main_camera->set_controls(ControlsConfig::ORBITCONTROLS);
        main_camera->set_position(glm::vec3(0.0f, 0.0f, 1.0f));
        set_camera(main_camera);
    }

    void render(double current_time, double delta_time) override
    {
        clear_color_buffer(bg_color);
        comp_mesh->render(basic_mat);
    }

    void shutdown() override
    {
        // cleanup meshes, materials, and Cameras
        delete text;
        delete basic_mat;
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