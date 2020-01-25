#ifndef SAMPLEUI1
#define SAMPLEUI1

#include "mare/Application.hpp"
#include "mare/Materials/PhongMaterial.hpp"
#include "mare/Meshes/SlopeMesh.hpp"
#include "mare/Meshes/CubeMesh.hpp"
#include "mare/CompositeMesh.hpp"
#include "mare/InstancedMesh.hpp"
#include "mare/Widgets/SliderUI.hpp"
#include "mare/Layer.hpp"

// create UI and run its startup, render and shutdown in here

namespace mare
{
class SampleUI1 : public Layer
{
public:
    SampleUI1()
    {
        // Create the camera and controls
        set_camera(new Camera(CameraType::ORTHOGRAPHIC));
        get_camera()->set_controls(ControlsConfig::NONE);
        get_camera()->set_position(glm::vec3(0.0f, 0.0f, 1.0f));

        // Create Widgets
        slider = new SliderUI(get_camera());
        on_resize(Application::get_input());
    }

    bool render(double time, double dt) override
    {
        // Renderer properties
        Application::enable_depth_testing(false);
        Application::enable_face_culling(true);

        slider->render(this);

        // Run forever
        return true;
    }

    bool on_mouse_button(const RendererInput &input) override
    {
        return slider->on_mouse_button(input);
    }

    bool on_mouse_move(const RendererInput &input) override
    {
        return slider->on_mouse_move(input);
    }
    bool on_resize(const RendererInput &input) override
    {
        glm::ivec2 screen_size = glm::ivec2(Application::get_info().window_width, Application::get_info().window_height);
        glm::vec3 world_size = get_camera()->screen_to_world(screen_size);
        slider->set_position({-world_size.x + 0.6f, -0.9f, 0.0f});
        return false;
    }

    ~SampleUI1()
    {
        // cleanup (widgets cleanup their objects)
        delete slider;
    }

private:
    SliderUI *slider;
};
} // namespace mare

#endif