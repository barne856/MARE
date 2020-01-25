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
    virtual ~CompositeMesh();
    void render(Layer* layer, Material *material) override;
    void render(Layer* layer, Material *material, glm::mat4 parent_model) override;
    void render(Layer* layer, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) override;
    void push_mesh(Mesh *mesh) { m_meshes.push_back(mesh); }

protected:
    std::vector<Mesh *> m_meshes{};
};
} // namespace mare

#endif