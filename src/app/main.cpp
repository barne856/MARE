#include "mare/Application.hpp"

#include <iostream>
#include <vector>

// TODO:
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
    glm::vec4 tri_color{0.95f, 0.95f, 0.95f, 1.0f};
    Mesh *tri;
    Mesh* tri2;
    Mesh* tri3;
    CompositeMesh* comp;
    CompositeMesh* comp2;
    Material *basic_mat;
    Camera *main_camera;
    bool wireframe = false;
    CharMesh* some_text;

    InstancedMesh* instance_mesh;

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
        tri = GenTriangle({-0.5f, -0.5f}, {0.5f, -0.5f}, {0.5f, 0.5f});
        tri2 = GenTriangle({-0.5f, -0.5f}, {0.5f, -0.5f}, {0.5f, 0.5f});
        tri3 = GenTriangle({-0.5f, -0.5f}, {0.5f, -0.5f}, {0.5f, 0.5f});
        tri->set_scale(glm::vec3(0.25f));
        tri2->set_scale(glm::vec3(0.25f));
        tri3->set_scale(glm::vec3(0.25f));
        tri2->translate({0.0f, 0.0f, 0.1f});
        tri3->translate({0.0f, 0.0f, 0.2f});
        basic_mat = GenMaterial("./res/Shaders/Basic");
        main_camera = new Camera(CameraType::PERSPECTIVE);
        main_camera->set_controls(ControlsConfig::ORBITCONTROLS);
        main_camera->set_position(glm::vec3(0.0f, 0.0f, 1.0f));
        set_camera(main_camera);

        comp = GenCompositeMesh();
        comp2 = GenCompositeMesh();
        comp->translate({0.0f, 0.0f, -0.05f});
        comp->push_mesh(tri);
        comp->push_mesh(tri2);
        comp2->push_mesh(comp);
        comp2->push_mesh(tri3);
        comp2->translate({0.0f, 0.0f, 0.05f});
        basic_mat->bind();
        basic_mat->upload_vec4("u_color", tri_color);

        std::vector<glm::mat4> instances {};
        instances.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(-0.25f, 0.0f, 0.0f)));
        instances.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.25f, 0.0f, 0.0f)));
        instances.push_back(glm::translate(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 0.0f)));

        instance_mesh = GenInstancedMesh(3);
        instance_mesh->set_mesh(tri);
        instance_mesh->push_instance(instances[0]);
        instance_mesh->push_instance(instances[1]);
        instance_mesh->push_instance(instances[2]);

        some_text = GenCharMesh("M.A.R.E.");
        some_text->set_scale({0.15f, 0.3f, 0.3f});
        some_text->set_center(glm::vec3(0.0f));

    }

    void render(double current_time, double delta_time) override
    {
        clear_color_buffer(bg_color);
        //comp2->render(basic_mat);
        //instance_mesh->render(basic_mat);
        some_text->render(basic_mat);
    }

    void shutdown() override
    {
        // cleanup meshes, materials, and Cameras
        delete tri;
        delete tri2;
        delete tri3;
        delete comp;
        delete comp2;
        delete basic_mat;
        delete main_camera;
    }

    void on_key(const RendererInput& input) override
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