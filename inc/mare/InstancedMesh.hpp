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
    InstancedMesh() {}
    virtual ~InstancedMesh() {}
    virtual void render(Material *material) = 0;
    virtual void render(Material *material, glm::mat4 model) = 0;

protected:
    unsigned int instance_count{};
    std::vector<glm::mat4> instance_transforms{};
};
} // namespace mare

#endif