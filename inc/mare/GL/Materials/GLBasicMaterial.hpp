#ifndef GLBASICMATERIAL
#define GLBASICMATERIAL

// MARE
#include "mare/GL/GLMaterial.hpp"
#include "mare/Materials/BasicMaterial.hpp"

namespace mare
{
class GLBasicMaterial : public GLMaterial, public BasicMaterial
{
public:
    GLBasicMaterial()
    {
        m_shader.create("./res/Shaders/Basic");
    }
    virtual ~GLBasicMaterial() {}
    virtual void render() override
    {
        upload_uniform_vec4("u_color", m_color);
    }
};
} // namespace mare

#endif