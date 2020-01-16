#include "mare/GL/GLInstancedMesh.hpp"
#include "mare/Application.hpp"

namespace mare
{

GLInstancedMesh::GLInstancedMesh(unsigned int max_instances)
    : InstancedMesh(max_instances)
{
    instance_transforms = new GLBuffer<glm::mat4>();
    std::vector<glm::mat4> empty{};
    instance_transforms->create(empty, max_instances * sizeof(glm::mat4));
}

GLInstancedMesh::~GLInstancedMesh()
{
    delete instance_transforms;
}

void GLInstancedMesh::push_instance(glm::mat4 model)
{
    std::vector<glm::mat4> models{model};
    instance_transforms->update(models, instance_count);
    instance_count++;
}

void GLInstancedMesh::render(Material *material)
{
    m_mesh->render(material, transform, instance_count, instance_transforms);
}

void GLInstancedMesh::render(Material *material, glm::mat4 parent_model)
{
    m_mesh->render(material, transform * parent_model, instance_count, instance_transforms);
}

void GLInstancedMesh::render(Material *material, glm::mat4 parent_model, unsigned int instance_count, Buffer<glm::mat4> *models)
{
    // dunno how to render and instanced mesh of an instanced mesh
}

} // namespace mare