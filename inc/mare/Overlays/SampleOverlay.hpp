#ifndef SAMPLEOVERLAY
#define SAMPLEOVERLAY

#include "mare/Renderer.hpp"
#include "mare/Widgets/SliderUI.hpp"
#include "mare/Overlay.hpp"

namespace mare
{
class SampleOverlay : public Overlay
{
public:
    SampleOverlay() : Overlay(ProjectionType::ORTHOGRAPHIC)
    {
        // Create Widgets
        gen_widget<SliderUI>(this);
        get_widget<SliderUI>()->set_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
        get_widget<SliderUI>()->set_value(0.5f);
    }

    void on_enter() override
    {
    }

    void render(float delta_time) override
    {
        // Renderer properties
        Renderer::enable_depth_testing(false);
        Renderer::enable_face_culling(true);
    }

    void on_exit() override
    {
    }
};
} // namespace mare

#endif