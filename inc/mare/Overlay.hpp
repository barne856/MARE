#ifndef OVERLAY
#define OVERLAY

// MARE
#include "mare/Layer.hpp"
#include "mare/Widget.hpp"
#include "mare/Application.hpp"

// Standard Library
#include <vector>

namespace mare
{
class Overlay : public Layer
{
public:
    Overlay() : m_widget_stack(nullptr)
    {
        m_widget_stack = new std::vector<Widget *>();
    }
    virtual ~Overlay()
    {
        for (size_t i = m_widget_stack->size(); i--;)
        {
            delete m_widget_stack->at(i);
            m_widget_stack->at(i) = nullptr;
        }
        delete m_widget_stack;
        m_widget_stack = nullptr;
    }
    bool render(double time, double dt) override
    {
        // Renderer properties
        Application::enable_depth_testing(false);
        Application::enable_face_culling(true);

        // render widgets
        for (auto &widget : *m_widget_stack)
        {
            widget->render(this);
        }

        // Run forever
        return true;
    }
    bool on_key(const RendererInput &input) override
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
    bool on_mouse_button(const RendererInput &input) override
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

    bool on_mouse_move(const RendererInput &input) override
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

    bool on_mouse_wheel(const RendererInput &input) override
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

    Widget *get_widget(size_t index)
    {
        return (*m_widget_stack)[index];
    }
    void push_widget(Widget *widget)
    {
        m_widget_stack->push_back(widget);
    }

private:
    std::vector<Widget *> *m_widget_stack;
};
} // namespace mare

#endif