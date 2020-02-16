#ifndef OVERLAY
#define OVERLAY

// MARE
#include "mare/Layer.hpp"
#include "mare/Widget.hpp"
#include "mare/Renderer.hpp"

// Standard Library
#include <vector>

namespace mare
{
class Overlay : public Layer
{
public:
    Overlay();
    virtual ~Overlay();
    
    bool render(double time, double dt) final;

    bool on_key(const RendererInput &input) final;
    bool on_mouse_button(const RendererInput &input) final;
    bool on_mouse_move(const RendererInput &input) final;
    bool on_mouse_wheel(const RendererInput &input) final;
    virtual bool on_resize(const RendererInput &input) = 0;

    Widget *get_widget(size_t index);
    void push_widget(Widget *widget);

private:
    std::vector<Widget *> *m_widget_stack;
};
} // namespace mare

#endif