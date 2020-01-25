#ifndef SAMPLESCENE
#define SAMPLESCENE

#include "mare/Application.hpp"

#include "mare/Scene.hpp"
#include "mare/Objects/MareTextObject.hpp"
#include "mare/Overlays/SampleOverlay.hpp"

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

        // Push overlays to the layer stack
        push_layer(new SampleOverlay());
    }
    ~SampleScene()
    {
        delete mare_text;
        mare_text = nullptr;
    }

    bool render(double time, double dt) override
    {
        // Renderer properties
        Application::enable_depth_testing(true);
        Application::enable_face_culling(true);

        // get UI values
        float v = std::get<float>(get_widget_value(0, 0));
        mare_text->set_light_intensity(v);

        // Render objects
        mare_text->render(this);

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
    bool wireframe = false;
};
} // namespace mare

#endif