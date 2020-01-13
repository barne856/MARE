#ifndef CONTROLS
#define CONTROLS

namespace mare
{

class Camera; // forward declaration
struct RendererInput; // forward declaration

enum class ControlsConfig
{
    ORBITCONTROLS
};

class Controls
{
public:
    virtual ~Controls(){}
    virtual void interpret_input(Camera *camera, RendererInput &input) = 0;
};

class OrbitControls : public Controls
{
public:
    void interpret_input(Camera *camera, RendererInput &input) override;
};

} // namespace mare

#endif