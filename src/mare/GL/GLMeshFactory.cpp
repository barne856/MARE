#include "mare/GL/GLMeshFactory.hpp"
#include "mare/GL/Meshes/GLTriangleMesh.hpp"

namespace mare
{
Mesh *GLMeshFactory::GenTriangle(glm::vec2 v1, glm::vec2 v2, glm::vec2 v3)
{
    return new GLTriangleMesh(v1, v2, v3);
}
} // namespace mare