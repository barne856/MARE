#ifndef COMPOSITEMESH
#define COMPOSITEMESH

// Standard Library
#include <vector>
// MARE
#include "mare/Mesh.hpp"
#include "mare/MareUtility.hpp"

namespace mare
{
class CompositeMesh : virtual public Mesh
{
public:
    CompositeMesh() {}
    virtual ~CompositeMesh() {}
    void render(const Layer *layer, Material *material) override;
    void render(const Layer *layer, Material *material, const glm::mat4 &parent_model) override;
    void render(const Layer *layer, Material *material, const glm::mat4 &parent_model, unsigned int instance_count, const Buffer<glm::mat4> *models) override;
    void push_mesh(Scoped<Mesh> mesh); // push a mesh onto the mesh stack
    void pop_mesh();                   // remove the last mesh on the mesh stack
    void clear();                      // clear the entire mesh stack

protected:
    std::vector<Scoped<Mesh>> meshes_{};
};
} // namespace mare

#endif