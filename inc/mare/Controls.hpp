#ifndef CONTROLS
#define CONTROLS

#include "mare/MareUtility.hpp"

namespace mare
{

class Camera; // forward declaration
struct RendererInput; // forward declaration

class Controls
{
public:
    virtual ~Controls(){}
    virtual void interpret_input(Camera* camera, const RendererInput &input) = 0;
};

class OrbitControls : public Controls
{
public:
    void interpret_input(Camera* camera, const RendererInput &input) override;
};

class FlyControls : public Controls
{
public:
    void interpret_input(Camera* camera, const RendererInput &input) override;
};

} // namespace mare

#endif