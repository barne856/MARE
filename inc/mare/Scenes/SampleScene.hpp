#ifndef SAMPLESCENE
#define SAMPLESCENE

// MARE
#include "mare/Renderer.hpp"
#include "mare/Scene.hpp"
#include "mare/Overlays/SampleOverlay.hpp"
#include "mare/Systems.hpp"
#include "mare/Components/Controls/FlyControls.hpp"
#include "mare/Entities/SampleTorus.hpp"
#include "mare/Components/Rendering/ShadowMap.hpp"
#include "mare/Entities/SampleCube.hpp"

namespace mare
{

// forward declare componetent
class SampleSceneControls;

class SampleScene : public Scene
{
public:
    SampleScene() : Scene(ProjectionType::PERSPECTIVE)
    {
        // Set the components
        push_component<Rigidbody>();
        push_component<FlyControls>();
        push_component<SampleSceneControls>();
        push_component<ShadowMap>(8);
        set_position(glm::vec3(0.0f, 0.0f, 1.0f));
        face_towards(glm::vec3(0.0f, 0.2f, -1.0f));

        // Push entities
        push_entity(Renderer::API->GenScoped<SampleTorus>(100, 200, 0.1f, 0.2f));
        push_entity(Renderer::API->GenScoped<SampleCube>());

        // Push overlays to the layer stack
        push_overlay(Renderer::API->GenScoped<SampleOverlay>());
    }

    void on_enter() override
    {
        // Start with the cursor disabled for Fly Contorls
        Renderer::API->set_cursor(CURSOR::DISABLED);
    }

    void render(float delta_time) override
    {
        // Renderer properties
        Renderer::API->enable_blending(true);
        Renderer::API->enable_depth_testing(true);
        Renderer::API->enable_face_culling(true);
        // Clear color and depth buffer
        Renderer::API->clear_color_buffer(bg_color);
        Renderer::API->clear_depth_buffer();

        // get SliderUI value and scale torus
        float v = std::get<float>(get_overlay(0)->get_widget(0)->get_value());
        get_entity(0)->set_scale(glm::vec3(2.0f * v + 0.05f));
    }

    void on_exit() override
    {
        // Show cursor on new scenes
        Renderer::API->set_cursor(CURSOR::ENABLED);
    }

private:
    glm::vec4 bg_color{0.9f, 0.9f, 0.9f, 1.0f};
};

class SampleSceneControls : public ControlsSystem<SampleScene>
{
public:
    bool on_key(SampleScene *scene, const RendererInput &input) override
    {
        if (input.T_JUST_PRESSED)
        {
            Renderer::API->get_info().wireframe = !(Renderer::API->get_info().wireframe);
        }
        Renderer::API->wireframe_mode(Renderer::API->get_info().wireframe);
        // show mouse and disable controls
        if (input.LEFT_CONTROL_PRESSED)
        {
            Renderer::API->set_cursor(CURSOR::ENABLED);
            scene->pull_component<FlyControls>();
            scene->get_component<Rigidbody>()->linear_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
            return true;
        }
        else
        {
            Renderer::API->set_cursor(CURSOR::DISABLED);
            scene->push_component<FlyControls>();
        }
        // event is handled
        return false;
    }
    bool on_mouse_button(SampleScene *scene, const RendererInput &input) override
    {
        if (input.mouse_button == 1)
        {
            Renderer::API->get_info().focus = scene;
            if (input.LEFT_CONTROL_PRESSED)
            {
                scene->get_entity(0)->set_position(Renderer::API->raycast(scene));
            }
            return true;
        }
        Renderer::API->get_info().focus = nullptr;
        return false;
    }
};

} // namespace mare

#endif