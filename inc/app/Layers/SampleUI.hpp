#ifndef SAMPLEUI
#define SAMPLEUI

#include "mare/Renderer.hpp"
#include "mare/Entities/UI/SliderUI.hpp"
#include "mare/Layer.hpp"

namespace mare
{
class SampleUI : public Layer
{
public:
    SampleUI() : Layer(ProjectionType::ORTHOGRAPHIC)
    {
        // Create Widgets
        gen_entity<SliderUI>(this);
        get_entity<SliderUI>()->set_color(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
        get_entity<SliderUI>()->set_value(0.5f);
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