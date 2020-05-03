#ifndef OVERLAY
#define OVERLAY

// MARE
#include "mare/Mare.hpp"
#include "mare/Layer.hpp"
#include "mare/Widget.hpp"

// Standard Library
#include <vector>

namespace mare
{
    class Overlay : public Layer
    {
    public:
        Overlay(ProjectionType type) : Layer(type) {}
        virtual ~Overlay() {}

        template <typename T, typename... Args>
        void push_widget(Args... args)
        {
            Referenced<Widget> widget = std::dynamic_pointer_cast<Widget>(Renderer::API->GenRef<T>(args...));
            if (widget)
            {
                widget_stack_.push_back(widget);
            }
            // nothing pushed if T is not an widget
        }
        template <typename T>
        Referenced<T> pull_widget()
        {
            for (auto widget_it = widget_begin(); widget_it != widget_end(); widget_it++)
            {
                if (auto widget = std::dynamic_pointer_cast<T>(*widget_it))
                {
                    Referenced<T> pulled_widget = std::move(widget);
                    widget_stack_.erase(widget_it);
                    return pulled_widget;
                }
            }
            return nullptr;
        }
        void pop_widget()
        {
            widget_stack_.pop_back();
        }
        template <typename T>
        T *get_widget()
        {
            for (auto widget_it = widget_begin(); widget_it != widget_end(); widget_it++)
            {
                if (auto widget = std::dynamic_pointer_cast<T>(*widget_it))
                {
                    return widget.get();
                }
            }
            return nullptr;
        }
        template <typename T>
        std::vector<T *> get_widgets()
        {
            std::vector<T *> widgets{};
            for (auto widget_it = widget_begin(); widget_it != widget_end(); widget_it++)
            {
                if (auto widget = std::dynamic_pointer_cast<T>(*widget_it))
                {
                    widgets.push_back(widget.get());
                }
            }
            return widgets;
        }

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