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
    void push_mesh(Mesh *mesh); // push a mesh onto the mesh stack
    void pop_mesh(); // remove the last mesh on the mesh stack
    void clear(); // clear the entire mesh stack
    void flush(std::vector<Mesh*> meshes); // set the entire mesh stack, makes a copy of meshes

protected:
    std::vector<Mesh *> m_meshes{};
};
} // namespace mare

#endif