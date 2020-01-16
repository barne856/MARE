#ifndef INSTANCEDMESH
#define INSTANCEDMESH

// Standard Library
#include <vector>
// MARE
#include "mare/Mesh.hpp"

namespace mare
{
class InstancedMesh : virtual public Mesh
{
public:
    InstancedMesh(unsigned int max_instances) : instance_count(0), instance_transforms(nullptr), m_mesh(nullptr), m_max_instances(max_instances) {}
    virtual ~InstancedMesh() {}
    virtual void render(Material *material) = 0;
    virtual void render(Material *material, glm::mat4 parent_model) = 0;
    virtual void render(Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) = 0;
    void set_mesh(Mesh *mesh) { m_mesh = mesh; }
    virtual void push_instance(glm::mat4 model) = 0;

protected:
    unsigned int instance_count;
    Buffer<glm::mat4> *instance_transforms;
    Mesh *m_mesh;
    unsigned int m_max_instances;
};
} // namespace mare

#endif