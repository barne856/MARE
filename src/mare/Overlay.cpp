// MARE
#include "mare/Overlay.hpp"

namespace mare
{

Overlay::Overlay() : m_widget_stack(nullptr)
{
    m_widget_stack = new std::vector<Widget *>();
}
Overlay::~Overlay()
{
    for (size_t i = m_widget_stack->size(); i--;)
    {
        delete m_widget_stack->at(i);
        m_widget_stack->at(i) = nullptr;
    }
    delete m_widget_stack;
    m_widget_stack = nullptr;
}
bool Overlay::render(double time, double dt)
{
    // Renderer properties
    Renderer::API->enable_depth_testing(false);
    Renderer::API->enable_face_culling(true);

    // render widgets
    for (auto &widget : *m_widget_stack)
    {
        widget->render(this);
    }

    // Run forever
    return true;
}
bool Overlay::on_key(const RendererInput &input)
{
    for (size_t i = m_widget_stack->size(); i--;)
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
    for (size_t i = m_widget_stack->size(); i--;)
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
    for (size_t i = m_widget_stack->size(); i--;)
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
    for (size_t i = m_widget_stack->size(); i--;)
    {
        if (get_widget(i)->on_mouse_wheel(input))
        {
            return true;
        }
    }
    return false;
}

Widget *Overlay::get_widget(size_t index)
{
    return (*m_widget_stack)[index];
}
void Overlay::push_widget(Widget *widget)
{
    m_widget_stack->push_back(widget);
}
} // namespace mare
