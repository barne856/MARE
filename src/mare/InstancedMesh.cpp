#include "mare/InstancedMesh.hpp"
#include "mare/Application.hpp"

namespace mare
{

InstancedMesh::InstancedMesh(unsigned int max_instances)
    : instance_count(0), instance_transforms(nullptr), m_mesh(nullptr), m_max_instances(max_instances)
{
    instance_transforms = Application::GenBuffer<glm::mat4>(1);
    std::vector<glm::mat4> empty{};
    instance_transforms->create(empty, max_instances * sizeof(glm::mat4));
}

InstancedMesh::~InstancedMesh()
{
    if (instance_transforms)
    {
        delete[] instance_transforms;
    }
    if(m_mesh)
    {
        delete m_mesh;
    }
}

void InstancedMesh::push_instance(glm::mat4 model)
{
    std::vector<glm::mat4> models{model};
    instance_transforms->update(models, instance_count);
    instance_count++;
}

void InstancedMesh::update_instance(unsigned int index, glm::mat4 model)
{
    instance_transforms->update(&model, (size_t)sizeof(glm::mat4), (unsigned int)(index*sizeof(glm::mat4)));
}

void InstancedMesh::render(Layer *layer, Material *material)
{
    m_mesh->render(layer, material, transform, instance_count, instance_transforms);
}

void InstancedMesh::render(Layer *layer, Material *material, glm::mat4 parent_model)
{
    m_mesh->render(layer, material, parent_model * transform, instance_count, instance_transforms);
}

void InstancedMesh::render(Layer *layer, Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models)
{
    // dunno how to render and instanced mesh of an instanced mesh
}

} // namespace mare