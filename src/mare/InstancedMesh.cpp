#include "mare/InstancedMesh.hpp"
#include "mare/Renderer.hpp"

namespace mare
{

InstancedMesh::InstancedMesh(unsigned int max_instances)
    : instance_count(0), instance_transforms(nullptr), m_mesh(nullptr), m_max_instances(max_instances)
{
    instance_transforms = Renderer::API->GenMat4Buffer(nullptr, BufferType::READ_WRITE, max_instances * sizeof(glm::mat4));
}

InstancedMesh::~InstancedMesh()
{
    if (instance_transforms)
    {
        delete[] instance_transforms;
    }
    if (m_mesh)
    {
        delete m_mesh;
    }
}

void InstancedMesh::push_instance(glm::mat4 model)
{
    (*instance_transforms)[instance_count] = model;
    instance_count++;
}

void InstancedMesh::pop_instance()
{
    instance_count--;
}

void InstancedMesh::clear_instances()
{
    instance_count = 0;
}

void InstancedMesh::flush_instances(std::vector<glm::mat4> &models, size_t offset)
{
    instance_transforms->flush(models, offset);
}

glm::mat4 &InstancedMesh::operator[](unsigned int i)
{
    return (*instance_transforms)[i];
}

glm::mat4 InstancedMesh::operator[](unsigned int i) const
{
    return (*instance_transforms)[i];
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
    // rendering an instanced mesh of instanced meshes is a bad idea. It will not reduce draw calls and it will
    // read from the models buffer in a very ineffiecnt way, only render instances of simple meshes or composite meshes consisting only of simple meshes in their mesh trees.
    for (unsigned int i = 0; i < instance_count; i++)
    {
        m_mesh->render(layer, material, (*models)[i]);
    }
}

} // namespace mare