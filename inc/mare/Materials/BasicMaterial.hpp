#ifndef BASICMATERIAL
#define BASICMATERIAL

// External Libraries
#include "glm.hpp"

// MARE
#include "mare/Material.hpp"

namespace mare
{
class BasicMaterial : public virtual Material
{
public:
    BasicMaterial() : Material("./res/Shaders/Basic"), m_color(glm::vec4(0.0f)) {}
    virtual ~BasicMaterial() {}
    void render() override
    {
        m_shader->upload_vec4("u_color", m_color);
    }
    inline void set_color(glm::vec4 color) { m_color = color; }

protected:
    glm::vec4 m_color;
};
} // namespace mare

#endif