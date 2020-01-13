#ifndef GLMATERIAL
#define GLMATERIAL

// Standard Library
#include <unordered_map>

// MARE
#include "mare/Material.hpp"
#include "mare/GL/GLShader.hpp"

#include "glm.hpp"

namespace mare
{
class GLMaterial : public virtual Material
{
public:
    GLMaterial(const char *directory) { m_shader = new GLShader(directory); }
    virtual ~GLMaterial() { delete m_shader; }
    virtual void render() {}
    void bind() final override { m_shader->use(); }
};
} // namespace mare

#endif