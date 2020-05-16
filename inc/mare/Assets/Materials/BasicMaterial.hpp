#ifndef BASICMATERIAL
#define BASICMATERIAL

// External Libraries
#include "glm.hpp"

// MARE
#include "mare/Shader.hpp"

namespace mare
{
class BasicMaterial : public virtual Material
{
public:
    BasicMaterial() : Material("./inc/mare/Assets/Shaders/Basic"), m_color(glm::vec4(1.0f)) {}
    virtual ~BasicMaterial() {}
    void render() override
    {
        upload_vec4("u_color", m_color);
    }
    inline void set_color(glm::vec4 color) { m_color = color; }

protected:
    glm::vec4 m_color;
};
} // namespace mare

#endif