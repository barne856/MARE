// MARE
#include "mare/Overlay.hpp"

namespace mare
{
Overlay::Overlay(ProjectionType type) : Layer(type) {}
Overlay::~Overlay() {}
void Overlay::push_widget(Referenced<Widget> widget)
{
    for (auto widget_it = widget_begin(); widget_it != widget_end(); widget_it++)
    {
        if (*widget_it == widget)
        {
            return;
        }
    }
    widget_stack_.push_back(widget);
    widget->set_layer(this);
}
void Overlay::pop_widget()
{
    widget_stack_.pop_back();
}
Widget* Overlay::get_widget(size_t index)
{
    return widget_stack_.at(index).get();
}
} // namespace mare
