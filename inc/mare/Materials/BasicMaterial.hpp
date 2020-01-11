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
    BasicMaterial() : m_color(glm::vec4(0.0f)) {}
    virtual ~BasicMaterial() {}
    virtual void render() = 0;
    inline void set_color(glm::vec4 color) { m_color = color; }

protected:
    glm::vec4 m_color;
};
} // namespace mare

#endif