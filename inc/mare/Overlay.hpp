#ifndef OVERLAY
#define OVERLAY

// MARE
#include "mare/MareUtility.hpp"
#include "mare/Layer.hpp"
#include "mare/Widget.hpp"

// Standard Library
#include <vector>

namespace mare
{
class Overlay : public Layer
{
public:
    Overlay(ProjectionType type);
    virtual ~Overlay();
    void push_widget(Referenced<Widget> widget);
    void pop_widget();
    Widget* get_widget(size_t index);

    std::vector<Referenced<Widget>>::const_iterator widget_begin() const { return widget_stack_.begin(); }
    std::vector<Referenced<Widget>>::const_iterator widget_end() const { return widget_stack_.end(); }
    std::vector<Referenced<Widget>>::const_reverse_iterator widget_rbegin() const { return widget_stack_.rbegin(); }
    std::vector<Referenced<Widget>>::const_reverse_iterator widget_rend() const { return widget_stack_.rend(); }
    std::vector<Referenced<Widget>>::iterator widget_begin() { return widget_stack_.begin(); }
    std::vector<Referenced<Widget>>::iterator widget_end() { return widget_stack_.end(); }
    std::vector<Referenced<Widget>>::reverse_iterator widget_rbegin() { return widget_stack_.rbegin(); }
    std::vector<Referenced<Widget>>::reverse_iterator widget_rend() { return widget_stack_.rend(); }

private:
    std::vector<Referenced<Widget>> widget_stack_;
};
} // namespace mare

#endif