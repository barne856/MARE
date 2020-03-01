#ifndef LAYER
#define LAYER

// MARE
#include "mare/Entities/Camera.hpp"

namespace mare
{
class Layer : public Camera
{
public:
    Layer(ProjectionType type) : Camera(type) {}
    virtual ~Layer() {}
    virtual void on_enter() = 0;
    virtual void render(float delta_time) = 0;
    virtual void on_exit() = 0;
};
} // namespace mare

#endif