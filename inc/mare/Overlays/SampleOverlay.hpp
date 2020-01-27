#ifndef SAMPLEOVERLAY
#define SAMPLEOVERLAY

#include "mare/Application.hpp"
#include "mare/Materials/PhongMaterial.hpp"
#include "mare/Meshes/SlopeMesh.hpp"
#include "mare/Meshes/CubeMesh.hpp"
#include "mare/CompositeMesh.hpp"
#include "mare/InstancedMesh.hpp"
#include "mare/Widgets/SliderUI.hpp"
#include "mare/Overlay.hpp"

namespace mare
{
class SampleOverlay : public Overlay
{
public:
    SampleOverlay()
    {
        // Create the camera and controls
        set_camera(new Camera(CameraType::ORTHOGRAPHIC));
        get_camera()->set_controls(ControlsConfig::NONE);
        get_camera()->set_position(glm::vec3(0.0f, 0.0f, 1.0f));

        // Create Widgets
        push_widget(new SliderUI(this));
        on_resize(Application::get_input());
        get_widget(0)->set_value(0.5f);
    }


    bool on_resize(const RendererInput &input) override
    {
        // position widgets on the screen
        glm::ivec2 screen_size = glm::ivec2(Application::get_info().window_width, Application::get_info().window_height);
        glm::vec3 world_size = get_camera()->screen_to_world(screen_size);
        get_widget(0)->set_position({-world_size.x + 0.6f, -0.9f, 0.0f});
        return false;
    }

};
} // namespace mare

#endif