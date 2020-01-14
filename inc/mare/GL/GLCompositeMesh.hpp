#ifndef GLCOMPOSITEMESH
#define GLCOMPOSITEMESH

// Standard Library
#include <vector>
// MARE
#include "mare/GL/GLMesh.hpp"
#include "mare/CompositeMesh.hpp"

namespace mare
{
class GLCompositeMesh : public GLMesh, public CompositeMesh
{
public:
    GLCompositeMesh(){}
    virtual ~GLCompositeMesh() {}
    virtual void render(Material *material) override;
    virtual void render(Material *material, glm::mat4 model) override;
};
} // namespace mare

#endif