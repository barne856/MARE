#ifndef OVERLAY
#define OVERLAY

// MARE
#include "mare/Layer.hpp"
#include "mare/Mare.hpp"
#include "mare/Widget.hpp"


// Standard Library
#include <vector>

namespace mare {
class Overlay : public Layer {
public:
  Overlay(ProjectionType type) : Layer(type) {}
  virtual ~Overlay() {}

  // Widget Template Functions--------------------------------

  // generate single widget and push it onto the stack
  template <typename T, typename... Args> void gen_widget(Args... args) {
    // generate widget reference of type T and cast to base type
    Referenced<Widget> widg =
        std::dynamic_pointer_cast<Widget>(gen_ref<T>(args...));
    // if T is a widget push the widget onto the stack
    if (widg)
      widgets_.push_back(widg);
  }
  // generate multiple widgets and push them onto the stack
  template <typename T, typename... Args>
  void gen_widgets(uint32_t count, Args... args) {
    for (uint32_t i = 0; i < count; i++) {
      // generate widget reference of type T and cast to base type
      Referenced<Widget> widg =
          std::dynamic_pointer_cast<Widget>(gen_ref<T>(args...));
      // if T is a widget push the widget onto the stack
      if (widg)
        widgets_.push_back(widg);
    }
  }
  // get first widget in the stack of a given type
  template <typename T> T *get_widget() {
    for (auto widg_it = widget_begin(); widg_it != widget_end(); widg_it++) {
      if (auto widget = std::dynamic_pointer_cast<T>(*widg_it)) {
        return widget.get();
      }
    }
    return nullptr;
  }
  // get all widgets of an interface type
  template <typename T> std::vector<T *> get_widgets() {
    std::vector<T *> widgets{};
    for (auto widg_it = widget_begin(); widg_it != widget_end(); widg_it++) {
      if (auto widget = std::dynamic_pointer_cast<T>(*widg_it)) {
        widgets.push_back(widget.get());
      }
    }
    return widgets;
  }
  // push an existing reference onto the stack
  template <typename T> void push_widget(Referenced<T> widget) {
    Referenced<Widget> widg = std::dynamic_pointer_cast<Widget>(widget);
    if (widg)
      widgets_.push_back(widg);
  }
  // push a vector of existing references onto the stack
  template <typename T>
  void push_widgets(std::vector<Referenced<T>> widgets) {
    for (size_t i = 0; i < widgets.size(); i++) {
      Referenced<Widget> widg = std::dynamic_pointer_cast<Widget>(widgets[i]);
      if (widg)
        widgets_.push_back(widg);
    }
  }
  // remove first widget of a given type from the stack and return it
  template <typename T> Referenced<T> pull_widget() {
    for (auto widg_it = widget_begin(); widg_it != widget_end(); widg_it++) {
      if (auto widget = std::dynamic_pointer_cast<T>(*widg_it)) {
        Referenced<T> pulled_widget = std::move(widget);
        widgets_.erase(widg_it);
        return pulled_widget;
      }
    }
    return nullptr;
  }
  // remove all widgets of a given type from the stack and return them as a
  // vector of references
  template <typename T> std::vector<Referenced<T>> pull_widgets() {
    std::vector<T *> widgets{};
    for (auto widg_it = widget_begin(); widg_it != widget_end(); widg_it++) {
      if (auto widget = std::dynamic_pointer_cast<T>(*widg_it)) {
        Referenced<T> pulled_widget = std::move(widget);
        widgets_.erase(widg_it);
        widgets.push_back(pulled_widget);
      }
    }
    return widgets;
  }

  std::vector<Referenced<Widget>>::const_iterator widget_begin() const {
    return widgets_.begin();
  }
  std::vector<Referenced<Widget>>::const_iterator widget_end() const {
    return widgets_.end();
  }
  std::vector<Referenced<Widget>>::const_reverse_iterator
  widget_rbegin() const {
    return widgets_.rbegin();
  }
  std::vector<Referenced<Widget>>::const_reverse_iterator widget_rend() const {
    return widgets_.rend();
  }
  std::vector<Referenced<Widget>>::iterator widget_begin() {
    return widgets_.begin();
  }
  std::vector<Referenced<Widget>>::iterator widget_end() {
    return widgets_.end();
  }
  std::vector<Referenced<Widget>>::reverse_iterator widget_rbegin() {
    return widgets_.rbegin();
  }
  std::vector<Referenced<Widget>>::reverse_iterator widget_rend() {
    return widgets_.rend();
  }

private:
  std::vector<Referenced<Widget>> widgets_;
};
} // namespace mare

#endif