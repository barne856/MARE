#ifndef SAMPLESCENE
#define SAMPLESCENE

#include "mare/Application.hpp"

#include "mare/Scene.hpp"
#include "mare/Objects/MareTextObject.hpp"
#include "mare/Meshes/TorusMesh.hpp"
#include "mare/Meshes/CubeMesh.hpp"
#include "mare/Overlays/SampleOverlay.hpp"
#include "mare/Materials/PhongMaterial.hpp"
#include "mare/Materials/BasicMaterial.hpp"

#include "GL/glew.h"
#include <iostream>

namespace mare
{
class SampleScene : public Scene
{
public:
    SampleScene()
    {
        // Create the camera and controls
        set_camera(new Camera(CameraType::PERSPECTIVE));
        get_camera()->set_controls(ControlsConfig::ORBITCONTROLS);
        get_camera()->set_position(glm::vec3(0.0f, 0.0f, 1.0f));

        // create objects
        mare_text = new MareTextObject();
        torus = new TorusMesh(100, 200, 0.1f, 0.2f);
        cube = new CubeMesh(1.0f);
        cube->set_scale({5.0f, 5.0f, 0.05f});
        cube->set_position({0.0f, 0.0f, -0.5f});
        material = new PhongMaterial();
        shadow_material = new BasicMaterial();

        // Push overlays to the layer stack
        push_layer(new SampleOverlay());

        // OpenGL Depth Texture
        glGenTextures(1, &depth_texture);
        glBindTexture(GL_TEXTURE_2D, depth_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, Application::get_info().window_width, Application::get_info().window_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
        // OpenGL Framebuffer
        glGenFramebuffers(1, &depth_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);
        glDrawBuffer(GL_NONE);
        if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
        {
            std::cerr << "Framebuffer is incomplete" << std::endl;
            if (GL_FRAMEBUFFER_UNDEFINED == glCheckFramebufferStatus(GL_FRAMEBUFFER))
            {
                std::cerr << "Framebuffer is undefined" << std::endl;
            }
            if (GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT == glCheckFramebufferStatus(GL_FRAMEBUFFER))
            {
                std::cerr << "Framebuffer has incomplete attchment" << std::endl;
            }
            if (GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT == glCheckFramebufferStatus(GL_FRAMEBUFFER))
            {
                std::cerr << "Framebuffer has missing attachment" << std::endl;
            }
            if (GL_FRAMEBUFFER_UNSUPPORTED == glCheckFramebufferStatus(GL_FRAMEBUFFER))
            {
                std::cerr << "Framebuffer is unsupported" << std::endl;
            }
            if (GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE == glCheckFramebufferStatus(GL_FRAMEBUFFER))
            {
                std::cerr << "Framebuffer is incomplete (multisample)" << std::endl;
            }
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        shadow_camera = new Camera(CameraType::PERSPECTIVE);

        scale_bias_matrix = glm::mat4(glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
                                      glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
                                      glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
                                      glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    }
    ~SampleScene()
    {
        delete mare_text;
        delete material;
        delete torus;
        delete cube;
        mare_text = nullptr;
    }

    bool render(double time, double dt) override
    {
        // Renderer properties
        Application::enable_depth_testing(true);
        Application::enable_face_culling(true);

        // get UI values
        float v = std::get<float>(get_widget_value(0, 0));
        torus->set_scale(glm::vec3(2.0f * v + 0.05f));
        //material->light.ambient = glm::vec4(glm::vec3(v), 1.0f);
        //material->light.diffuse = glm::vec4(glm::vec3(v), 1.0f);
        //material->light.specular = glm::vec4(glm::vec3(v), 1.0f);

        // Render into depth buffer
        shadow_camera->set_view(material->get_light_position(), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shadow_material->bind();
        shadow_material->render();
        shadow_material->upload_mat4("projection", shadow_camera->projection());
        shadow_material->upload_mat4("view", shadow_camera->view());
        glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
        glViewport(0, 0, Application::get_info().window_width, Application::get_info().window_height);
        glClearDepth(1.0f);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(2.0f, 4.0f);
        Camera* temp_cam = get_camera();
        set_camera(nullptr);
        torus->render(this, shadow_material);
        cube->render(this, shadow_material);
        glDisable(GL_POLYGON_OFFSET_FILL);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        set_camera(temp_cam);

        // Render objects
        //mare_text->render(this);
        material->bind();
        material->render();
        material->upload_mat4("shadow_matrix", scale_bias_matrix * shadow_camera->projection() * shadow_camera->view());
        glBindTextureUnit(1, depth_texture);
        torus->render(this, material);
        cube->render(this, material);

        // Run forever
        return true;
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
    // Objects
    MareTextObject *mare_text = nullptr;
    TorusMesh *torus = nullptr;
    CubeMesh *cube = nullptr;
    PhongMaterial *material = nullptr;
    BasicMaterial *shadow_material = nullptr;
    Camera *shadow_camera = nullptr;
    glm::mat4 scale_bias_matrix;
    bool wireframe = false;
    GLuint depth_texture;
    GLuint depth_fbo;
};
} // namespace mare

#endif