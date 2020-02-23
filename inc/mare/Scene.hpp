#ifndef SCENE
#define SCENE

// MARE
#include "mare/MareUtility.hpp"
#include "mare/Layer.hpp"
#include "mare/Overlay.hpp"
#include "mare/Object.hpp"

// Standard Library
#include <vector>

namespace mare
{
class Scene : public Layer
{
public:
    Scene();
    virtual ~Scene();

    virtual bool render(double current_time, double delta_time) override { return true; }
    virtual void on_load(){}; // called when loaded if already in the cache
    virtual bool on_key(const RendererInput &input) { return false; }
    virtual bool on_mouse_button(const RendererInput &input) { return false; }
    virtual bool on_mouse_move(const RendererInput &input) { return false; }
    virtual bool on_mouse_wheel(const RendererInput &input) { return false; }
    virtual bool on_resize(const RendererInput &input) { return false; }

    shader_data_type get_widget_value(size_t overlay_index, size_t widget_index);
    Overlay *get_overlay(size_t index);
    void push_overlay(Scoped<Overlay> overlay);

    std::vector<Scoped<Overlay>>::const_iterator begin();
    std::vector<Scoped<Overlay>>::const_iterator end();
    std::vector<Scoped<Overlay>>::const_reverse_iterator rbegin();
    std::vector<Scoped<Overlay>>::const_reverse_iterator rend();

private:
    std::vector<Scoped<Overlay>> overlay_stack_{};
};
} // namespace mare

#endif