// MARE
#include "mare/Renderer.hpp"
#include "mare/Overlay.hpp"

namespace mare
{

Overlay::Overlay() {}
Overlay::~Overlay() {}
bool Overlay::render(double time, double dt)
{
    // Renderer properties
    Renderer::API->enable_depth_testing(false);
    Renderer::API->enable_face_culling(true);

    // render widgets
    for (auto &widget : widget_stack_)
    {
        widget->render(this);
    }

    // Continue Rendering
    return true;
}
bool Overlay::on_key(const RendererInput &input)
{
    for (size_t i = widget_stack_.size(); i--;)
    {
        if (get_widget(i)->on_key(input))
        {
            return true;
        }
    }
    return false;
}
bool Overlay::on_mouse_button(const RendererInput &input)
{
    for (size_t i = widget_stack_.size(); i--;)
    {
        if (get_widget(i)->on_mouse_button(input))
        {
            return true;
        }
    }
    return false;
}

bool Overlay::on_mouse_move(const RendererInput &input)
{
    for (size_t i = widget_stack_.size(); i--;)
    {
        if (get_widget(i)->on_mouse_move(input))
        {
            return true;
        }
    }
    return false;
}

bool Overlay::on_mouse_wheel(const RendererInput &input)
{
    for (size_t i = widget_stack_.size(); i--;)
    {
        if (get_widget(i)->on_mouse_wheel(input))
        {
            return true;
        }
    }
    return false;
}

Widget* Overlay::get_widget(size_t index)
{
    return widget_stack_[index].get();
}
void Overlay::push_widget(Scoped<Widget> widget)
{
    widget_stack_.push_back(std::move(widget));
}
} // namespace mare
