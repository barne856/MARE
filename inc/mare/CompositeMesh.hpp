#ifndef COMPOSITEMESH
#define COMPOSITEMESH

// Standard Library
#include <vector>
// MARE
#include "mare/Mesh.hpp"

namespace mare
{
class CompositeMesh : virtual public Mesh
{
public:
    CompositeMesh() {}
    virtual ~CompositeMesh() {}
    virtual void render(Material *material) = 0;
    virtual void render(Material *material, glm::mat4 model) = 0;
    void push_mesh(Mesh *mesh) { m_meshes.push_back(mesh); }

protected:
    std::vector<Mesh *> m_meshes{};
};
} // namespace mare

#endif