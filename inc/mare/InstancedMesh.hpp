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
    virtual ~InstancedMesh();
    void render(Layer *layer, Material *material) override;
    void render(Layer *layer, Material *material, glm::mat4 parent_model) override;
    void render(Layer *layer, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models) override;
    void set_mesh(Mesh *mesh) { m_mesh = mesh; }
    void push_instance(glm::mat4 model);
    void pop_instance();
    void clear_instances();
    void flush_instances(std::vector<glm::mat4> &models, size_t offset);
    glm::mat4 &operator[](unsigned int i);
    glm::mat4 operator[](unsigned int i) const;

protected:
    unsigned int instance_count;
    Buffer<glm::mat4> *instance_transforms;
    Mesh *m_mesh;
    unsigned int m_max_instances;
};
} // namespace mare

#endif