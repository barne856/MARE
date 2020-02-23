#include "mare/InstancedMesh.hpp"
#include "mare/Renderer.hpp"

namespace mare
{

InstancedMesh::InstancedMesh(unsigned int max_instances)
    : instance_count_(0), instance_transforms_(nullptr), mesh_(nullptr), max_instances_(max_instances)
{
    instance_transforms_ = Renderer::API->GenMat4Buffer(nullptr, BufferType::READ_WRITE, max_instances * sizeof(glm::mat4));
}

void InstancedMesh::set_mesh(Scoped<Mesh> mesh)
{
    mesh_ = std::move(mesh);
}

void InstancedMesh::push_instance(glm::mat4 model)
{
    (*instance_transforms_)[instance_count_] = model;
    instance_count_++;
}

void InstancedMesh::pop_instance()
{
    instance_count_--;
}

void InstancedMesh::clear_instances()
{
    instance_count_ = 0;
}

void InstancedMesh::flush_instances(std::vector<glm::mat4> &models, size_t offset)
{
    instance_transforms_->flush(models, offset);
}

glm::mat4 &InstancedMesh::operator[](unsigned int i)
{
    return (*instance_transforms_)[i];
}

glm::mat4 InstancedMesh::operator[](unsigned int i) const
{
    return (*instance_transforms_)[i];
}

void InstancedMesh::render(const Layer *layer, Material *material)
{
    mesh_->render(layer, material, transform_, instance_count_, instance_transforms_.get());
}

void InstancedMesh::render(const Layer *layer, Material *material, const glm::mat4 &parent_model)
{
    mesh_->render(layer, material, parent_model * transform_, instance_count_, instance_transforms_.get());
}

void InstancedMesh::render(const Layer *layer, Material *material, const glm::mat4 &parent_model, unsigned int instance_count, const Buffer<glm::mat4> *models)
{
    // rendering an instanced mesh of instanced meshes is a bad idea. It will not reduce draw calls and it will
    // read from the models buffer in a very ineffiecnt way, only render instances of simple meshes or composite meshes consisting only of simple meshes in their mesh trees.
    for (unsigned int i = 0; i < instance_count; i++)
    {
        mesh_->render(layer, material, (*models)[i]);
    }
}

} // namespace mare