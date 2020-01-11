#ifndef INSTANCEDMESH
#define INSTANCEDMESH

// Standard Library
#include <vector>
// MARE
#include "mare/Mesh.hpp"

namespace mare
{
class InstancedMesh : public Mesh
{
public:
    virtual ~InstancedMesh() {}
    virtual void render() = 0;

private:
    unsigned int instance_count {};
    std::vector<glm::mat4> instance_transforms {};
};
} // namespace mare

#endif