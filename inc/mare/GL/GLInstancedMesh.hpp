#ifndef GLINSTANCEDMESH
#define GLINSTANCEDMESH

// Standard Library
#include <vector>
// MARE
#include "mare/GL/GLMesh.hpp"
#include "mare/InstancedMesh.hpp"

namespace mare
{
class GLInstancedMesh : public GLMesh, public InstancedMesh
{
public:
    GLInstancedMesh(unsigned int max_instances);
    virtual ~GLInstancedMesh();
    void render(Material *material) override;
    void render(Material *material, glm::mat4 model) override;
    void render(Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4>* models) override;
    void push_instance(glm::mat4 model) override;
};
} // namespace mare

#endif