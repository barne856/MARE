#ifndef LAYER
#define LAYER

// MARE
#include "mare/MareUtility.hpp"
#include "mare/Camera.hpp"

namespace mare
{
struct RendererInput; // forward declaration
class Layer : public Asset
{
public:
    Layer() : camera_(nullptr) {}
    virtual ~Layer() {}

    virtual bool render(double current_time, double delta_time) = 0;

    virtual bool on_key(const RendererInput &input) = 0;
    virtual bool on_mouse_button(const RendererInput &input) = 0;
    virtual bool on_mouse_move(const RendererInput &input) = 0;
    virtual bool on_mouse_wheel(const RendererInput &input) = 0;
    virtual bool on_resize(const RendererInput &input) = 0;

    inline void set_camera(Scoped<Camera> camera) { camera_ = std::move(camera); }
    inline Camera *get_camera() const { return camera_.get(); }

private:
    Scoped<Camera> camera_;
};
} // namespace mare

#endif