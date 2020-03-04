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
    InstancedMesh(unsigned int max_instances);
    virtual ~InstancedMesh() {}
    void render(const Layer *layer, Material *material) override;
    void render(const Layer *layer, Material *material, const glm::mat4 &parent_model) override;
    void render(const Layer *layer, Material *material, const glm::mat4 &parent_model, unsigned int instance_count, const Buffer<glm::mat4> *models) override;
    void set_mesh(Scoped<Mesh> mesh);
    void push_instance(glm::mat4 model);
    void pop_instance();
    void clear_instances();
    void flush_instances(std::vector<glm::mat4> *models, size_t offset);
    glm::mat4 &operator[](unsigned int i);
    glm::mat4 operator[](unsigned int i) const;

protected:
    unsigned int instance_count_;
    Scoped<Buffer<glm::mat4>> instance_transforms_;
    Scoped<Mesh> mesh_;
    unsigned int max_instances_;
};
} // namespace mare

#endif