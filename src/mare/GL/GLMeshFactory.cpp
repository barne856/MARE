#include "mare/GL/GLMeshFactory.hpp"
#include "mare/GL/Meshes/GLTriangleMesh.hpp"
#include "mare/GL/Meshes/GLCharMesh.hpp"
#include "mare/GL/GLCompositeMesh.hpp"
#include "mare/GL/GLInstancedMesh.hpp"

namespace mare
{
Mesh *GLMeshFactory::GenTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
{
    return new GLTriangleMesh(v1, v2, v3);
}
CompositeMesh* GLMeshFactory::GenCompositeMesh()
{
    return new GLCompositeMesh();
}
InstancedMesh* GLMeshFactory::GenInstancedMesh(unsigned int max_instances)
{
    return new GLInstancedMesh(max_instances);
}
CharMesh* GLMeshFactory::GenCharMesh(std::string str, float keying)
{
    return new GLCharMesh(str, keying);
}
} // namespace mare